defmodule Astrum.Rest.Middleware do
  defmacro __using__(_opts) do
    quote do
      # @behaviour Ridehealth.Rest.Middleware

      @spec handle_request(
              opts :: any(),
              req :: Raxx.Request.t(),
              state :: any()
            ) :: {req :: Raxx.Request.t(), state :: any()}
      def handle_request(_opts, req, state), do: {:next, {req, state}}

      @spec handle_response(
              opts :: any(),
              req :: Raxx.Request.t(),
              resp :: Raxx.Request.t(),
              state :: any()
            ) :: any()
      def handle_response(_opts, _req, resp, _state), do: resp

      defoverridable handle_request: 3, handle_response: 4
    end
  end
end

defmodule Astrum.Rest.Middleware.Reverse do
  use Astrum.Rest.Middleware

  def handle_request(opts, req, state) do
    {:next, {%{req | path: Enum.reverse(req.path)}, state}}
  end
end

defmodule Astrum.Rest.Middleware.Decode do
  use Astrum.Rest.Middleware

  def handle_request(_opts, req = %{body: body}, state) when body == "" do
    req = %{req | query: URI.decode_query(req.query || "")}
    req = Map.put(req, :raw_body, req.body)
    {:next, {req, state}}
  end

  def handle_request(_opts, req = %{body: body}, state) do
    req = %{req | query: URI.decode_query(req.query || "")}
    req = Map.put(req, :raw_body, req.body)
    content_type = Raxx.get_header(req, "content-type")

    cond do
      String.starts_with?(content_type, "application/x-www-form-urlencoded") ->
        {:next, {%{req | body: URI.decode_query(body)}, state}}

      String.starts_with?(content_type, "application/json") ->
        body
        |> Jason.decode()
        |> case do
          {:ok, decoded} ->
            {:next, {%{req | body: decoded}, state}}

          _ ->
            {:abort, {:error, :invalid_json}}
        end

      true ->
        {:next, {req, state}}
    end
  end
end

defmodule Astrum.Rest.Middleware.Encode do
  use Astrum.Rest.Middleware

  def handle_request(_opts, req, state), do: {:next, {req, state}}

  def handle_response(_opts, _req, {:error, {error, body}}, _state) do
    400
    |> Raxx.response()
    |> set_body(%{error: error, body: body})
  end

  def handle_response(_opts, _req, {:error, error}, _state)
      when error in [:not_found] do
    404
    |> Raxx.response()
    |> set_body(%{error: error})
  end

  def handle_response(_opts, _req, {:error, error}, _state)
      when error in [:auth_invalid_email, :auth_invalid_secret, :auth_missing] do
    401
    |> Raxx.response()
    |> set_body(%{error: error})
  end

  def handle_response(_opts, _req, {:error, error}, _state) do
    400
    |> Raxx.response()
    |> set_body(%{error: error})
  end

  def handle_response(_opts, _req, {:ok, value}, _state) do
    200
    |> Raxx.response()
    |> set_body(value)
  end

  def handle_response(_opts, _req, resp, _state) do
    resp
  end

  def set_body(req, body) do
    req
    |> Raxx.set_body(Jason.encode!(body, pretty: true))
    |> Raxx.set_header("content-type", "application/json")
  end
end

defmodule Astrum.Rest.Middleware.Logger do
  use Astrum.Rest.Middleware
  require Logger

  def handle_request(_opts, req, state) do
    Logger.info("#{req.method} #{req.raw_path}")
    {:next, {req, state}}
  end

  def handle_response(_opts, _req, resp, _state) do
    Logger.info("#{resp.status}")
    resp
  end
end
