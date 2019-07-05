alias Astrum.Ledger

defmodule Astrum.Rest.Stripe do
  use Astrum.Rest
  middleware(Astrum.Rest.Middleware.Decode)
  middleware(Astrum.Rest.Middleware.Encode)

  def handle(%{path: ["hook" | _], body: body}, state) do
    customer = Dynamic.get(body, ["data", "object", "customer"])

    {:ok,
     %{
       metadata: %{
         "key" => user
       }
     }} = Stripe.Customer.retrieve(customer)

    body
    |> Dynamic.get(["data", "object", "display_items"], [])
    |> Stream.map(fn item ->
      {sku(item), item}
    end)
    |> Stream.map(fn
      {"dust", %{"quantity" => quantity}} ->
        Ledger.create(user, quantity, Astrum.User.root(), %{})

      {"plot." <> plot, %{"amount" => amount}} ->
        Kora.Mutation.merge(["plot:info", plot], %{
          "owner" => user,
          "prices" => %{
            "sold" => amount,
            "list" => 0
          }
        })
    end)
    |> Kora.Mutation.combine()
    |> Kora.mutation!()

    {:ok, true}
  end

  def sku(%{"custom" => %{"description" => description}}) do
    [_, type] = Regex.run(~r/sku:(.+)/, description)
    type
  end
end
