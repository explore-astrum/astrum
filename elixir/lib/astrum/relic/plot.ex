defmodule Astrum.Relic.Plot do
	use Kora.Interceptor
	alias Kora.Mutation
	alias Astrum.Plot

	def before_mutation(["user:relic:positions", _user, relic, key], %{
		merge: merge = %{
			"x" => x,
			"y" => y,
			"z" => _z,
			"pitch" => _pitch,
			"yaw" => _yaw,
			"roll" => _roll,
		}
	}, _, _) do
		plot = Plot.key_from_coord(x, y)
		{:combine, Mutation.merge(["plot:relic:positions", plot, relic, key], merge)}
	end

	def before_mutatoin(["user:relic:positions", _, _, _], _layer, _user, _mut) do
		{:error, :invalid_position}
	end
end