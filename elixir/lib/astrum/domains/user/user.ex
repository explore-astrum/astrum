alias Kora.Mutation

defmodule Astrum.User do
  require Dynamic

  Dynamic.schema(%{
    "key" => nil,
    "username" => nil,
    "email" => nil
  })

  def from_email(email) do
    case Kora.query_path!(["email:user", email]) do
      nil -> {:error, :email_unused}
      result -> {:ok, result}
    end
  end

  def from_username(username) do
    case Kora.query_path!(["username:user", username]) do
      nil -> {:error, :username_unused}
      result -> {:ok, result}
    end
  end

  def info(key) do
    Kora.query_path!(["user:info", key])
  end

  def uuid() do
    "USR" <> Kora.UUID.ascending()
  end

  def password_set(key, password) do
    hash = Comeonin.Bcrypt.hashpwsalt(password)
    Mutation.merge(["user:data", key, "hash"], hash)
  end

  def password_validate(key, password) do
    hash = Kora.query_path!(["user:data", key, "hash"])
    Comeonin.Bcrypt.checkpw(password, hash)
  end
end

defmodule Astrum.User.Data do
  require Dynamic

  Dynamic.schema(%{
    "key" => nil,
    "stripe" => %{
      "customer" => nil
    }
  })

  def get(user) do
    Kora.query_path!(["user:data", user])
  end
end
