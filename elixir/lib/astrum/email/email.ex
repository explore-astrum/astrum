defmodule Astrum.Email do
	alias Kora.Mutation
	alias Kora.UUID

	import Swoosh.Email

	def token(email, ref \\ nil) do
		["token:email", UUID.ascending()]
		|> Mutation.merge(%{
			"email" => email,
			"created" => :os.system_time(:millisecond)
		})
		|> Mutation.merge(["email:ref", email], ref)
	end

	def delete_token(token) do
		Mutation.delete(["token:email", token])
	end

	def from_token(token) do
		case Kora.query_path!(["token:email", token, "email"]) do
			nil -> {:error, :invalid_token}
			result -> {:ok, result}
		end
	end

	def send({subject, html}, to), do: send(to, subject, html)

	def send(to, subject, html) do
		new()
		|> to(to)
		|> from({"Astrum", "mail@exploreastrum.com"})
		|> subject(subject)
		|> html_body(html)
		|> Astrum.Email.Mailer.deliver
	end

end

defmodule Astrum.Email.Token do
	use Kora.Interceptor
	alias Astrum.Email

	def after_mutation(["token:email", token], %{merge: %{"email" => email}}, _mut, _user) do
		token
		|> Astrum.Email.Template.confirm(email)
		|> Email.send(email)
		:ok
	end
end

defmodule Astrum.Email.Mailer do
	use Swoosh.Mailer, otp_app: :astrum
end
