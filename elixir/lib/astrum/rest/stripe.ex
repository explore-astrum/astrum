defmodule Astrum.Rest.Stripe do
  use Astrum.Rest
  middleware(Astrum.Rest.Middleware.Decode)
  middleware(Astrum.Rest.Middleware.Encode)

  def handle(%{path: ["hook" | _], body: body}, state) do
    case body do
      %{
        "data" => %{
          "object" => %{
            "payment_intent" => payment_intent
          }
        }
      } ->
        payment_intent
        |> Stripe.PaymentIntent.retrieve(%{})
        |> case do
          {:ok,
           %{
             amount_received: price,
             metadata: %{
               "type" => "plot",
               "user" => user,
               "target" => target
             }
           }} ->
            Kora.Mutation.merge(["plot:info", target], %{
              "owner" => user,
              "prices" => %{
                "sold" => price / 100,
                "list" => 0
              }
            })
            |> Kora.mutation!()

            {:ok, "plot.sold"}
        end
    end
  end
end
