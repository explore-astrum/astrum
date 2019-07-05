alias Astrum.Plot
alias Astrum.User

defmodule Astrum.Command.Stripe do
  use Kora.Command

  def handle_command({"stripe.plot", plot, _}, _from, state = %{user: user}) do
    plot_info = Plot.info(plot)

    with {_, owner} when owner !== user <- {:owner, Plot.owner(plot_info)},
         {_, list} when list > 0 <- {:list, Plot.prices_list(plot_info)} do
      {x, y} = Plot.key_decode(plot)
      user_data = User.Data.get(user)

      owner_info = Astrum.User.info(owner)

      {:ok, %{id: id}} =
        Stripe.Session.create(%{
          payment_method_types: ["card"],
          success_url: "https://exploreastrum.com/success",
          cancel_url: "https://exploreastrum.com/cancel",
          customer: User.Data.stripe_customer(user_data),
          payment_intent_data: %{
            metadata: %{
              target: plot,
              type: "plot",
              user: user
            }
          },
          line_items: [
            %{
              name: "Plot: #{x}x#{y}",
              amount: list * 100,
              description: "You are purchasing this plot from @#{User.username(owner_info)}",
              currency: "usd",
              quantity: 1,
              images: Plot.images(plot)
            }
          ]
        })

      {:reply, id, state}
    else
      {:owner, _} -> {:error, :plot_already_owned, state}
      {:list, _} -> {:error, :plot_not_listed, state}
    end
  end
end
