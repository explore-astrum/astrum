alias Kora.Mutation

defmodule Astrum.Plot.Activity do
  require Dynamic
  use Kora.Interceptor

  Dynamic.schema(%{
    "key" => nil,
    "sender" => nil,
    "type" => nil,
    "data" => nil
  })

  def before_mutation(
        ["plot:info", plot],
        %{merge: %{"owner" => owner, "prices" => %{"sold" => price}}},
        _mut,
        _user
      ) do
    {:combine, create(plot, owner, "plot.sold", %{"price" => price})}
  end

  def before_mutation(
        ["plot:info", plot],
        %{merge: %{"prices" => %{"list" => price}}},
        _mut,
        user
      ) do
    {:combine, create(plot, user, "plot.list", %{"price" => price})}
  end

  def uuid() do
    "ACT" <> Kora.UUID.descending()
  end

  @spec create(binary, any, any, any) :: %{delete: map, merge: map}
  def create(plot, sender, type, data) do
    uuid = uuid()

    Mutation.merge(["plot:activity", plot, uuid], %{
      "key" => uuid,
      "sender" => sender,
      "created" => :os.system_time(:millisecond),
      "type" => type,
      "data" => data
    })
  end
end
