require Logger

defmodule Astrum.Server do
  use GenServer

  def start_link(port) do
    GenServer.start_link(__MODULE__, [port])
  end

  def init(args) do
    :pg2.create(:clients)
    send(self(), :listen)
    {:ok, args}
  end

  def handle_info(:listen, args = [port]) do
    {:ok, listen} =
      :gen_tcp.listen(port, [:binary, packet: :line, active: :once, reuseaddr: true])

    loop(listen)
    {:noreply, args}
  end

  def handle_info(msg, state) do
    {:noreply, state}
  end

  def loop(listen) do
    {:ok, client} = :gen_tcp.accept(listen)
    {:ok, pid} = Astrum.Server.Socket.start_link(client)
    :pg2.join(:clients, pid)
    :gen_tcp.controlling_process(client, pid)
    loop(listen)
  end

  def broadcast(msg) do
    members = :pg2.get_members(:clients)
    data = Astrum.Packet.encode(msg)
    Enum.each(members, fn member -> send(member, {:send, data}) end)
  end
end
