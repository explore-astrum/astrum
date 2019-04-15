alias Kora.Mutation
alias Astrum.Relic

defmodule Astrum.Relic.Position do
  use Kora.Interceptor

  def effect(["relic:info", relic], %{merge: %{"position" => [x, y, z]}}, _mut, _user) do
    Astrum.Server.broadcast(Astrum.Packet.relic_position(relic, x, y, z))
    nil
  end
end
