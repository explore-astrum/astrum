defmodule Astrum.Server.Relic.Position do
  use Astrum.Server.Handler

  def handle({:relic_position, {relic, {x, y, z}}}, _user, _from, _socket) do
    Kora.merge!(["relic:info", relic, "position"], [x, y, z])
    :ok
  end
end
