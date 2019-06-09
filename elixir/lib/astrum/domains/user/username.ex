alias Kora.Mutation
alias Astrum.User

defmodule Astrum.User.Username.Duplicate do
  use Kora.Interceptor

  def before_mutation(["user:info", _key], %{merge: %{"username" => username}}, _mut, _user) do
    case User.from_username(username) do
      {:error, :username_unused} -> :ok
      _ -> {:error, :username_used}
    end
  end
end

defmodule Astrum.User.Username.Index do
  use Kora.Interceptor

  def before_mutation(["user:info", key], %{merge: %{"username" => username}}, _mut, _user) do
    previous =
      key
      |> User.info()
      |> User.username()

    {
      :combine,
      Mutation.combine([
        Mutation.merge(["username:user", username], key),
        if previous != nil do
          Mutation.delete(["username:user", previous])
        end
      ])
    }
  end
end
