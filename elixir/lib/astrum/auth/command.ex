defmodule Astrum.Auth.Command do
	use Kora.Command
	alias Astrum.User
	alias Astrum.Email
	alias Kora.{Mutation, UUID}

	def handle_command({"auth.email", %{
		"email" => email,
	}, _version}, _from, state) do
		email
		|> String.downcase
		|> Email.token()
		|> Kora.mutation
		{:reply, true, state}
	end

	def handle_command(msg = {"auth.login", %{
		"token" => token,
		"address" => address,
		"wif" => wif,
	}, _version}, from, state) do
		with \
			{:ok, email} <- Email.from_token(token),
			{:ok, user} <- User.from_email(email)
		do
			result = UUID.ascending()
			Mutation.combine([
				Email.delete_token(token),
				User.token(user, result)
			])
			|> Kora.mutation!
			|> IO.inspect
			{:reply, result, state}
		else
			{:error, {:not_found, email}} ->
				address
				|> User.create(wif, email)
				|> Kora.mutation!
				handle_command(msg, from, state)
			{:error, err} -> {:error, err, state}

		end
	end

	def handle_command({"auth.upgrade", token, _version}, _from, state) do
		case User.from_token(token) do
			{:ok, result} -> {:reply, result, %{user: result}}
			{:error, err} -> {:error, err, state}
		end
	end
end