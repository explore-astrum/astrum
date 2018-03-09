defmodule Astrum.Relic.Balance do
	use Kora.Interceptor
	alias Kora.Mutation
	alias Astrum.{Relic, Plot}

	def before_mutation(["user:relics", user, relic], %{merge: %{"balance" => balance}}, _user, _mut) do
		current =
			user
			|> Relic.positions(relic)
			|> Map.keys
			|> length
		{
			:combine,
			generate(user, relic, current, balance)
		}
	end

	def generate(user, relic, current, next) when next > current do
		current..(next - 1)
		|> Stream.map(&inspect/1)
		|> Stream.map(fn key ->
			value = %{
				"key" => key,
				"hidden" => true,

			}
			Mutation.new
			|> Mutation.merge(["user:relic:positions", user, relic, key], value)
		end)
		|> Mutation.combine
	end

	def generate(user, relic, current, next) when next < current do
		current..(next - 1)
		|> Stream.map(&inspect/1)
		|> Stream.map(fn key ->
			Kora.query_path!(["user:relic:positions", user, relic, key])
			|> case do
				%{"x" => x, "y" => y} ->
					plot = Plot.key_from_coord(x, y)
					Mutation.new
					|> Mutation.delete(["user:relic:positions", user, relic, key])
					|> Mutation.delete(["plot:relics", plot, relic, key])
				_ -> Mutation.new
			end
		end)
		|> Mutation.combine
	end

	def generate(_user, _relic, _current, _next), do: Mutation.new
end