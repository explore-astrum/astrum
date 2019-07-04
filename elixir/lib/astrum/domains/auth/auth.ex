defmodule Astrum.Auth do
  use Kora.Interceptor

  def validate(data, user, path, history) when is_map(data) do
    data
    |> Enum.flat_map(fn {key, value} ->
      next_path = path ++ [key]

      case validate_merge(next_path, value, user) do
        res = {:error, _err} ->
          [res]

        nil ->
          validate(value, user, next_path, history)

        res ->
          validate(value, user, next_path, res)
      end
    end)
    |> Stream.map(fn
      [] -> {:error, :not_allowed}
      res -> res
    end)
    |> Enum.to_list()
  end

  def validate(_, _, _, history) do
    [history]
  end

  def validate_merge(["plot:activity", _plot, _activity], %{"type" => type}, _user)
      when type in ["plot.offer"],
      do: :ok

  def validate_merge(["plot:activity", _plot, _activity], _, _user) do
    {:error, :plot_activity_bad}
  end

  def validate_merge(["plot:info", _plot, field | rest], _value, _user)
      when [field | rest] not in [
             ["name"],
             ["prices", "list"]
           ] do
    {:error, {:unmodifiable_field, field}}
  end

  def validate_merge(["plot:info", plot], _value, user) do
    ["plot:info", plot, "owner"]
    |> Kora.query_path!()
    |> case do
      ^user -> :ok
      _ -> {:error, :plot_unowned}
    end
  end

  def validate_merge(_, _, _), do: nil

  def require_owner(root, entity, user) do
    [root, entity, "owner"]
    |> Kora.query_path!()
    |> case do
      ^user -> :ok
      _ -> {:error, :plot_unowned}
    end
  end
end
