require Logger
alias Kora.Mutation

defmodule Astrum.Server.Socket do
  def start_link(socket) do
    GenServer.start_link(__MODULE__, [socket])
  end

  def init([socket]) do
    Logger.info("Starting socket")
    Logger.info(:inet.peername(socket) |> inspect())

    send(self(), :bootstrap)

    {:ok, socket}
  end

  def line(socket, data) do
    :gen_tcp.send(socket, data <> "\n")
  end

  def handle_info(:bootstrap, socket) do
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

    {:noreply, socket}
  end

  def handle_info({:tcp_closed, _}, state) do
    {:stop, :normal, state}
  end

  def handle_info({:send, data}, state) do
    # Logger.info("Sent #{inspect(data)}")
    line(state, data)
    {:noreply, state}
  end

  def handle_info({:tcp, _, data}, socket) do
    result = Astrum.Packet.decode(String.trim_trailing(data))
    Logger.info("Got #{inspect(result)}")
    :inet.setopts(socket, active: :once)
    {:noreply, socket}
  end

  def handle_info(msg, socket) do
    IO.inspect(msg)
    {:noreply, socket}
  end
end
