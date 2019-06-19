defmodule Astrum.Server.Handler do
  @callback handle(msg :: any, user :: any, from :: any, socket :: any) :: any

  def trigger(msg, user, from, socket) do
    Enum.map(Astrum.Config.astrum_handlers(), fn mod ->
      :ok = mod.handle(msg, user, from, socket)
    end)
  end

  def all() do
    for {module, _} <- :code.all_loaded(),
        __MODULE__ in (module.module_info(:attributes)[:behaviour] || []) do
      module
    end
  end

  defmacro __using__(_opts) do
    quote do
      @behaviour Astrum.Server.Handler
      @before_compile Astrum.Server.Handler
      require Logger
      import Astrum.Packet
      import Astrum.Server.Socket
    end
  end

  defmacro __before_compile__(_env) do
    quote do
      def handle(msg, user, from, socket), do: :ok
    end
  end
end
