defmodule Astrum.User.Master do
  use Kora.Interceptor

  def resolve_path(["user:info", "USR00000000000000000000" | rest], opts, user) do
    %{
      "key" => "USR00000000000000000000",
      "username" => "astrum"
    }
    |> Dynamic.get(rest)
  end
end
