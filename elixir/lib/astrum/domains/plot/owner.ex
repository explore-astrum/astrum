alias Kora.Mutation
alias Astrum.Plot

defmodule Astrum.Plot.Owner do
  use Kora.Interceptor

  def before_mutation(["plot:info", plot], %{merge: %{"owner" => owner}}, _, _) do
    {
      :combine,
      Mutation.combine([
        plot
        |> Plot.info()
        |> Plot.owner()
        |> case do
          nil -> nil
          previous -> Mutation.delete(["user:plots", previous, owner])
        end,
        Mutation.merge(["user:plots", owner, plot], %{
          "key" => plot
        })
      ])
    }
  end

  def effect(["plot:info", plot], %{merge: %{"owner" => owner}}, _mut, _user) do
    Astrum.Server.broadcast(Astrum.Packet.plot_owner(plot, owner))
    nil
  end
end
