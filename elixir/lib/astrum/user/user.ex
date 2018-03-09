defmodule Astrum.User do
	alias Kora.Mutation

	def create(address, wif, email) do
		Mutation.new
		|> Mutation.merge(["user:info", address], %{
			"key" => address,
		})
		|> Mutation.merge(["user:data", address], %{
			"key" => address,
			"email" => email,
			"wif" => wif,
		})
	end

	def token(user, token) do
		Mutation.merge(["token:user", token], user)
	end

	def from_email(email) do
		["email:user", email]
		|> Kora.query_path!
		|> case do
			nil -> {:error, {:not_found, email}}
			user -> {:ok, user}
		end
	end

	def from_token(token) do
		case Kora.query_path!(["token:user", token]) do
			nil -> {:error, :invalid_token}
			result -> {:ok, result}
		end
	end

end