alias Kora.Mutation
alias Astrum.Relic

defmodule Astrum.Relic.Owner do
  use Kora.Interceptor

  def before_mutation(["relic:info", relic], %{merge: %{"owner" => owner}}, _, _) do
    {
      :combine,
      Mutation.combine([
        relic
        |> Relic.info()
        |> Relic.owner()
        |> case do
          nil -> nil
          previous -> Mutation.delete(["user:relics", previous, owner])
        end,
        Mutation.merge(["user:relics", owner, relic], %{
          "key" => relic
        })
      ])
    }
  end

  def effect(["relic:info", relic], %{merge: %{"owner" => owner}}, _mut, _user) do
    Astrum.Server.broadcast({:relic_owner, {relic, owner}})
    nil
  end
end
