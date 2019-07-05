defmodule Astrum.Plot do
  require Dynamic

  Dynamic.schema(%{
    "key" => nil,
    "owner" => nil,
    "name" => nil,
    "prices" => %{
      "list" => nil
    }
  })

  def generate(x_range, y_range) do
    for x <- x_range,
        y <- y_range do
      create(x, y)
      |> Kora.Mutation.merge(["plot:info", key_encode(x, y), "prices", "list"], 3000)
    end
    |> Kora.Mutation.combine()
  end

  def info(key) do
    Kora.query_path!(["plot:info", key])
  end

  def create(x, y) do
    key = key_encode(x, y)

    Kora.Mutation.merge(["plot:info", key], %{
      "key" => key,
      "owner" => "USR00000000000000000000"
    })
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

  def images(plot) do
    [
      "nw",
      "sw",
      "se",
      "ne"
    ]
    |> Enum.map(fn direction ->
      "https://public.exploreastrum.com/iso/#{plot}#{direction}.jpg"
    end)
  end
end
