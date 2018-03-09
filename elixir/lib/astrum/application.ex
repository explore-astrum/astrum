defmodule Astrum.Application do
  # See https://hexdocs.pm/elixir/Application.html
  # for more information on OTP Applications
  @moduledoc false

  use Application

  def start(_type, _args) do
    # List all child processes to be supervised
    Astrum.Config.load(Fig.Loader.Env)
    Elixir.Application.put_env(:astrum, Astrum.Email.Mailer, [
      adapter: Swoosh.Adapters.Mailgun,
      api_key: Astrum.Config.mailgun_api_key(),
      domain: Astrum.Config.mailgun_domain()
    ])
    children = [
      # Starts a worker by calling: Astrum.Worker.start_link(arg)
      # {Astrum.Worker, arg},
      if Astrum.Config.postgres_hostname !== nil do
        Astrum.Config.postgres
        |> Map.put(:pool_size, 8)
        |> Map.to_list
        |> Kora.Store.Postgres.child_spec
      end,
      {Kora.Server, port: 12000},
      Astrum.Rest.child_spec(13000)
    ]
    |> Enum.filter(&(&1 != nil))

    # See https://hexdocs.pm/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: Astrum.Supervisor]
    Supervisor.start_link(children, opts)
  end
end
