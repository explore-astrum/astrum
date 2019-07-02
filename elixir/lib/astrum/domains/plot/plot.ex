defmodule Astrum.Plot do
  require Dynamic

  Dynamic.schema(%{
    "owner" => nil,
    "key" => nil
  })

  def generate(x_range, y_range) do
    for x <- x_range,
        y <- y_range do
      create(x, y)
    end
    |> Kora.Mutation.combine()
  end

  def buy(key, owner, price) do
    Kora.Mutation.merge(["plot:info", key], %{
      "owner" => owner,
      "prices" => %{
        "sold" => price,
        "list" => 0
      }
    })
  end

  def info(key) do
    Kora.query_path!(["plot:info", key])
  end

  def create(x, y) do
    key = key_encode(x, y)
    Kora.Mutation.merge(["plot:info", key, "key"], key)
  end

  @spec key_decode(binary) :: {integer, integer}
  def key_decode(key) do
    <<x::integer-signed-size(16), y::integer-signed-size(16)>> = Base.decode16!(key)
    {x, y}
  end

  def key_encode(x, y) do
    <<x::integer-signed-size(16), y::integer-signed-size(16)>>
    |> Base.encode16()
  end
end
