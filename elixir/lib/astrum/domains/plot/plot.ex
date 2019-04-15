defmodule Astrum.Plot do
  def key_decode(key) do
    <<x::integer-signed-size(16), y::integer-signed-size(16)>> = Base.decode16!(key)
    {x, y}
  end

  def key_encode(x, y) do
    <<x::integer-signed-size(16), y::integer-signed-size(16)>>
    |> Base.encode16()
  end
end
