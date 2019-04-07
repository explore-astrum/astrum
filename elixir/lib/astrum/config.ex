defmodule Astrum.Config do
  use Fig

  config :kora, %{
    interceptors: [
      Astrum.Relic.Create,
      Astrum.Relic.Owner,
      Astrum.Relic.Position
    ],
    commands: [],
    read: {Kora.Store.LMDB, directory: 'data'},
    writes: [{Kora.Store.LMDB, directory: 'data'}],
    scheduler: true,
    format: nil
  }
end
