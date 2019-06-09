alias Kora.Mutation
alias Astrum.User

defmodule Astrum.User.Email.Duplicate do
  use Kora.Interceptor

  def before_mutation(["user:info", _key], %{merge: %{"email" => email}}, _mut, _user) do
    case User.from_email(email) do
      {:error, :email_unused} -> :ok
      _ -> {:error, :email_used}
    end
  end
end

defmodule Astrum.User.Email.Index do
  use Kora.Interceptor

  def before_mutation(["user:info", key], %{merge: %{"email" => email}}, _mut, _user) do
    previous =
      key
      |> User.info()
      |> User.email()

    {
      :combine,
      Mutation.combine([
        Mutation.merge(["email:user", email], key),
        if previous != nil do
          Mutation.delete(["email:user", previous])
        end
      ])
    }
  end
end
