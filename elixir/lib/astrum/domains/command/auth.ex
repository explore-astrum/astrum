alias Astrum.User

defmodule Astrum.Command.Auth do
  use Kora.Command
  use Joken.Config

  def handle_command(
        {
          "auth.upgrade",
          token,
          _v
        },
        _from,
        state
      ) do
    case verify_and_validate(token) do
      {:ok, %{"user" => user}} ->
        {:reply, user, %{user: user}}

      _ ->
        {:error, :token_invalid, state}
    end
  end

  def handle_command(
        {
          "auth.login",
          %{
            "email" => email,
            "password" => password
          },
          _v
        },
        _from,
        state
      ) do
    with {:ok, user} <- User.from_email(email),
         true <- User.password_validate(user, password),
         {:ok, token, _} <-
           generate_and_sign(%{
             "user" => user
           }) do
      {:reply, token, state}
    else
      {:error, err} -> {:error, err, state}
    end
  end

  def handle_command(
        {"auth.register",
         %{
           "email" => email,
           "password" => password,
           "username" => username
         }, _},
        _from,
        state
      ) do
    key = User.uuid()
    email = String.downcase(email)
    username = String.downcase(username)

    Kora.Mutation.combine([
      Kora.Mutation.merge(["user:info", key], %{
        "key" => key,
        "email" => email,
        "username" => username
      }),
      User.password_set(key, password)
    ])
    |> Kora.mutation()
    |> case do
      {:error, err} ->
        {:error, err, state}

      _ ->
        {:reply, :ok, state}
    end
  end
end
