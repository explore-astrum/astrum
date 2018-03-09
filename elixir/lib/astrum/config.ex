defmodule Astrum.Config do
	use Fig

	config :logger, %{
		level: :error
	}

	config :kora, %{
		interceptors: [
			Astrum.Relic.Balance,
			Astrum.Relic.Plot,
			Astrum.Email.Token,

			Astrum.User.Email,
			Astrum.User.Bootstrap,
		],
		commands: [
			Astrum.Auth.Command
		],
		read: {Kora.Store.Level, directory: "kora.db"},
		writes: [{Kora.Store.Level, directory: "kora.db"}],
	}

	config :mailgun, %{
		api_key: nil,
		domain: nil
	}

	config :astrum, %{
		url: "http://localhost:3000",
		bootstrap: "http://localhost:8000"
	}

	config :postgres, %{
		hostname: nil,
		port: nil,
		username: nil,
		password: nil,
		database: nil,
		ssl: true,
	}

end
