defmodule Astrum.User.Bootstrap do
	use Kora.Interceptor
	alias Astrum.Config

	def before_mutation(["user:data", user], %{merge: %{"key" => _}}, _mut, _user), do: bootstrap(user)

	def bootstrap(address) do
		"#{Config.astrum_bootstrap()}/bootstrap/#{address}"
		|> HTTPoison.get
		|> case do
			{:ok, %{body: "true"}} -> :ok
			_ ->
				:timer.sleep(5_000)
				bootstrap(address)
		end
	end
	
end