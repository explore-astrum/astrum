require Logger

defmodule Astrum.User.Stripe.Customer do
  use Kora.Interceptor

  def effect(["user:info", key], %{merge: %{"key" => _}}, _mut, _user), do: {:create, [key]}

  @spec create(any) :: :ok
  def create(key) do
    Logger.info("Creating Stripe user")
    info = Astrum.User.info(key)

    {:ok, result} =
      Stripe.Customer.create(%{
        email: Astrum.User.email(info),
        metadata: %{
          "key" => key
        }
      })

    Kora.merge!(["user:info", key, "stripe", "customer"], result.id)

    :ok
  end
end
