require Logger

defmodule Astrum.Stripe.Ticker do
  use GenServer

  def start_link() do
    GenServer.start_link(__MODULE__, [])
  end

  def init(_) do
    Logger.info("Starting #{__MODULE__}")
    send(self(), :tick)
    {:ok, %{}}
  end

  def handle_info(:tick, state) do
    now = :os.system_time(:millisecond)

    ["kora:scheduler"]
    |> Kora.stream_path()
    |> Enum.each(fn
      {key,
       %{
         "timestamp" => timestamp
       }}
      when now >= timestamp ->
        Kora.Scheduler.execute(key)

      _ ->
        :skip
    end)

    Process.send_after(self(), :tick, 1000)
    {:noreply, state}
  end

  def handle_info({:swarm, :die}, state) do
    {:stop, :shutdown, state}
  end

  def handle_call({:swarm, :begin_handoff}, _from, state) do
    {:reply, :restart, state}
  end

  def handle_cast({:swarm, :end_handoff, _state}, state) do
    {:noreply, state}
  end

  def handle_cast({:swarm, :resolve_conflict, _state}, state) do
    {:noreply, state}
  end
end
