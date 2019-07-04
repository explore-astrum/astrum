defmodule Astrum.Config do
  use Fig

  config :kora, %{
    interceptors: [
      Astrum.Auth,
      Astrum.Relic.Create,
      Astrum.Relic.Owner,
      Astrum.Relic.Position,
      Astrum.Plot.Create,
      Astrum.Plot.Owner,
      Astrum.Plot.Activity,
      Astrum.User.Stripe.Customer,
      Astrum.User.Email.Duplicate,
      Astrum.User.Email.Index,
      Astrum.User.Username.Duplicate,
      Astrum.User.Username.Index
    ],
    commands: [
      Astrum.Command.Auth
    ],
    read: {Kora.Store.Memory, directory: "data/kora.json"},
    writes: [{Kora.Store.Memory, directory: "data/kora.json"}],
    scheduler: true,
    format: nil
  }

  config :stripity_stripe, %{
    api_key: "sk_test_6xDuznjQsIJJ7ig74W9zEwZq00jP0hDgV6"
  }

  config :joken, %{
    default_signer: "secret"
  }

  config :astrum, %{
    handlers: [
      Astrum.Server.Bootstrap,
      Astrum.Server.Ping,
      Astrum.Server.Relic.Position
    ]
  }
end
