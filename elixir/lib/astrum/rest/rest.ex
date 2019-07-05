defmodule Astrum.Rest do
  @callback handle(req :: Raxx.Request.t(), state :: any()) :: any

  defmacro middleware(item, opts \\ []) do
    quote do
      @middlewares {unquote(item), unquote(opts)}
    end
  end

  defmacro __before_compile__(_env) do
    quote do
      def middlewares(), do: @middlewares
    end
  end

  defmacro __using__(_opts) do
    quote do
      Module.register_attribute(__MODULE__, :middlewares, accumulate: true)
      import Astrum.Rest, only: [middleware: 1]
      @before_compile Astrum.Rest
      @behaviour Astrum.Rest
      middleware(Astrum.Rest.Middleware.Reverse)

      use Raxx.SimpleServer

      def handle_request(req, state) do
        {req, response, state} = process_request(req, state, middlewares())

        Enum.reduce(middlewares(), response, fn {mw, opts}, response ->
          mw.handle_response(opts, req, response, state)
        end)
      end

      def process_request(req, state, []) do
        {req, handle(req, state), state}
      end

      def process_request(req, state, [{mw, opts} | tail]) do
        mw.handle_request(opts, req, state)
        |> case do
          {:next, {next_req, next_state}} -> process_request(next_req, next_state, tail)
          {:abort, response} -> {req, response, state}
        end
      end
    end
  end
end
