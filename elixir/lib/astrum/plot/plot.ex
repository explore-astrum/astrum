defmodule Astrum.Plot do
	@plot_width 64
	@plot_height 64

	def from_coord(x, y) do
		{
			Integer.floor_div(x, @plot_width),
			Integer.floor_div(y, @plot_height),
		}
	end

	def key({x, y}), do: "#{x}:#{y}"

	def key_from_coord(x, y) do
		x
		|> from_coord(y)
		|> key
	end

	def from_key(input) do
		[x, y] = String.split(input, ":")
		{String.to_integer(x), String.to_integer(y)}
	end
end