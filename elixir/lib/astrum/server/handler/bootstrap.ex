require Logger
alias Kora.Mutation

defmodule Astrum.Server.Bootstrap do
  use Astrum.Server.Handler

  def handle({:bootstrap, _}, _user, _from, _socket) do
    Logger.info("Bootstrapping...")

    ["relic:info"]
    |> Kora.stream_path()
    |> Enum.each(fn {key, relic} ->
      [
        Astrum.Relic.Create,
        Astrum.Relic.Owner,
        Astrum.Relic.Position
      ]
      |> Kora.Interceptor.effect(Mutation.merge(["relic:info", key], relic), "kora-master")
    end)

    ["plot:info"]
    |> Kora.stream_path()
    |> Enum.each(fn {key, plot} ->
      [
        Astrum.Plot.Create,
        Astrum.Plot.Owner
      ]
      |> Kora.Interceptor.effect(Mutation.merge(["plot:info", key], plot), "kora-master")
    end)

    :ok
  end
end
