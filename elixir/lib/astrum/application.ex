defmodule Astrum.Application do
  # See https://hexdocs.pm/elixir/Application.html
  # for more information on OTP Applications
  @moduledoc false

  use Application

  def start(_type, _args) do
    Astrum.Config.load(Fig.Loader.Env)
    Application.put_env(:astrum, :handlers, Astrum.Server.Handler.all())
    # List all child processes to be supervised
    children = [
      # Starts a worker by calling: Astrum.Worker.start_link(arg)
      # {Astrum.Worker, arg}
      {Kora.Server, [port: 12000]},
      {Astrum.Server, 16000}
    ]

    # See https://hexdocs.pm/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: Astrum.Supervisor]
    Supervisor.start_link(children, opts)
  end
end
