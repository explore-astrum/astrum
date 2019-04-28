defmodule Astrum.Plot do
  require Dynamic

  Dynamic.schema(%{
    "owner" => nil,
    "key" => nil
  })

  def info(key) do
    Kora.query_path!(["plot:info", key])
  end

  def create(x, y) do
    key = key_encode(x, y)
    Kora.Mutation.merge(["plot:info", key, "key"], key)
  end

  def key_decode(key) do
    <<x::integer-signed-size(16), y::integer-signed-size(16)>> = Base.decode16!(key)
    {x, y}
  end

  def key_encode(x, y) do
    <<x::integer-signed-size(16), y::integer-signed-size(16)>>
    |> Base.encode16()
  end
end
