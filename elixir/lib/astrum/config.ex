defmodule Astrum.Config do
  use Fig

  config :kora, %{
    interceptors: [
      Astrum.Relic.Create,
      Astrum.Relic.Owner,
      Astrum.Relic.Position
    ],
    commands: [],
    read: {Kora.Store.Memory, directory: "data/kora.json"},
    writes: [{Kora.Store.Memory, directory: "data/kora.json"}],
    scheduler: true,
    format: nil
  }

  config :stripity_stripe, %{
    api_key: "sk_test_6xDuznjQsIJJ7ig74W9zEwZq00jP0hDgV6"
  }
end
