alias Kora.Mutation

defmodule Astrum.Relic do
  use Dynamic.Macros

  get([
    :owner,
    :position,
    :key,
    :type
  ])

  def create(type, name, count) do
    0..(count - 1)
    |> Stream.map(fn instance ->
      key = key_encode(type, instance)
      Mutation.merge(["relic:info", key, "key"], key)
    end)
    |> Mutation.combine()
    |> Mutation.merge(["relic:type", type, "name"], name)
  end

  def info(key) do
    Kora.query_path!(["relic:info", key])
  end

  def key_decode(key) do
    <<type::size(32), instance::size(32)>> = Base.decode16!(key)
    {type, instance}
  end

  def key_encode(type, instance) do
    <<type::size(32), instance::size(32)>>
    |> Base.encode16()
  end
end
