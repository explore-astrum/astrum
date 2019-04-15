require Logger
alias Kora.Mutation

defmodule Astrum.Server.Socket do
  def start_link(socket) do
    GenServer.start_link(__MODULE__, [socket])
  end

  def init([socket]) do
    Logger.info("Starting socket")
    Logger.info(:inet.peername(socket) |> inspect())

    {:ok, socket}
  end

  def line(socket, data) do
    :gen_tcp.send(socket, data <> "\n")
  end

  def handle_info(:bootstrap, socket) do
    IO.inspect("bbooty")
    Astrum.Server.Handler.trigger({:bootstrap, {}}, "master", self(), socket)
    {:noreply, socket}
  end

  def handle_info({:tcp_closed, _}, state) do
    {:stop, :normal, state}
  end

  def handle_info({:line, data}, socket) do
    # Logger.info("Sent #{inspect(data)}")
    line(socket, data)
    {:noreply, socket}
  end

  def handle_info({:tcp, _, data}, socket) do
    msg = Astrum.Packet.decode(String.trim_trailing(data))
    Logger.info("Got #{inspect(msg)} from #{inspect(data, limit: :infinity)}")
    pid = self()
    Task.start_link(fn -> Astrum.Server.Handler.trigger(msg, "master", pid, socket) end)
    :inet.setopts(socket, active: :once)
    {:noreply, socket}
  end

  def handle_info(_msg, socket) do
    {:noreply, socket}
  end
end
