defmodule Astrum.Server.Ping do
  use Astrum.Server.Handler

  def handle({:ping, _}, _user, _from, socket) do
    line(socket, pong())
    :ok
  end
end
