alias Astrum.Relic
alias Kora.Mutation

defmodule Astrum.Relic.Create do
  use Kora.Interceptor

  def before_mutation(["relic:info", relic], %{merge: %{"key" => _}}, _mut, _user) do
    {type, _} = Relic.key_decode(relic)

    {:combine,
     Mutation.new()
     |> Mutation.merge(["relic:info", relic], %{
       "created" => :os.system_time(:millisecond),
       "type" => type
     })
     |> Mutation.merge(["relic:type", type, "instances", relic], :os.system_time(:millisecond))}
  end

  def effect(["relic:info", relic], %{merge: %{"key" => _}}, _mut, _user) do
    {type, _key} = Relic.key_decode(relic)
    Astrum.Server.broadcast(Astrum.Packet.relic_create(type, relic))
    nil
  end
end
