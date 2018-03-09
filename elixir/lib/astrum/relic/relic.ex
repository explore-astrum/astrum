defmodule Astrum.Relic do
	def positions(user, relic) do
		["user:relic:positions", user, relic]
		|> Kora.query_path!
		|> Dynamic.default(%{})
	end

	def create(name) do
		key = Kora.UUID.ascending()
		["relic:info", key]
		|> Kora.merge!(%{
			"key" => key,
			"name" => name
		})
	end

end