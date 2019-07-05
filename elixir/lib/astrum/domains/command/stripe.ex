alias Astrum.Plot
alias Astrum.User

defmodule Astrum.Command.Stripe do
  use Kora.Command

  def handle_command({"stripe.checkout", txns, _}, _from, state = %{user: user}) do
    txns
    |> Stream.map(fn item -> validate(item, user) end)
    |> Enum.reduce_while([], fn
      err = {:error, _}, _collect ->
        {:halt, err}

      item, collect ->
        {:cont, collect ++ item}
    end)
    |> case do
      {:error, err} ->
        {:error, err, state}

      txns ->
        user_data = User.Data.get(user)

        {:ok, %{id: id}} =
          Stripe.Session.create(%{
            payment_method_types: ["card"],
            success_url: "https://exploreastrum.com/success",
            cancel_url: "https://exploreastrum.com/cancel",
            customer: User.Data.stripe_customer(user_data),
            payment_intent_data: %{
              metadata: %{
                user: user
              }
            },
            line_items: Enum.map(txns, &convert/1)
          })

        {:reply, id, state}
    end
  end

  def validate(tx = %{"type" => "plot", "target" => target}, user) do
    plot_info = Plot.info(target)

    with {_, owner} when owner !== user <- {:owner, Plot.owner(plot_info)},
         {_, list} when list > 0 <- {:list, Plot.prices_list(plot_info)} do
      cost =
        cond do
          owner == User.root() -> list - 1000
          true -> list
        end

      [
        Map.put(tx, "cost", cost),
        if owner === User.root() do
          %{"type" => "dust.bonus", "quantity" => 10, "cost" => 100}
        end
      ]
    else
      {:owner, _} -> {:error, :plot_owned}
      {:list, _} -> {:error, :plot_unavailable}
    end
  end

  def convert(%{"type" => "plot", "target" => plot, "cost" => cost}) do
    {x, y} = Plot.key_decode(plot)

    %{
      name: "Plot: #{x}x#{y}",
      amount: cost,
      description: "Your very own piece of Astrum. sku:plot.#{plot}",
      currency: "usd",
      quantity: 1
    }
  end

  def convert(%{"type" => "dust.bonus", "quantity" => quantity, "cost" => cost}) do
    %{
      name: "Bonus Dust",
      amount: cost,
      description: "Bonus currency for purchasing a plot from @astrum. sku:dust",
      currency: "usd",
      quantity: quantity
    }
  end
end
