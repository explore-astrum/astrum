alias Kora.Mutation
alias Astrum.Plot

defmodule Astrum.Plot.Create do
  use Kora.Interceptor

  def effect(["plot:info", plot], %{merge: %{"key" => _key}}, _mut, _user) do
    {x, y} = Plot.key_decode(plot)
    Astrum.Server.broadcast(Astrum.Packet.plot_create(x, y, plot))
    nil
  end
end
