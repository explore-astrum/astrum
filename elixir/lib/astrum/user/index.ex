defmodule Astrum.User.Email do
	use Kora.Interceptor
	alias Kora.Mutation
	alias Astrum.User

	# def validate_query(["user:data"], _, _, _) do
	# 	{:error, :not_allowed}
	# end

	def before_mutation(["user:data", key], %{merge: %{"email" => email}}, _mut, _user) do
		email
		|> User.from_email()
		|> case do
			{:ok, _} -> {:error, :email_exists}
			{:error, {:not_found, _email}} ->
				{:combine, Mutation.merge(["email:user", email], key)}
		end
	end
	
end