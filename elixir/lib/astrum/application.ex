defmodule Astrum.Application do
  # See https://hexdocs.pm/elixir/Application.html
  # for more information on OTP Applications
  @moduledoc false

  use Application

  def start(_type, _args) do
    Astrum.Config.load(Fig.Loader.Env)
    # List all child processes to be supervised
    children = [
      # Starts a worker by calling: Astrum.Worker.start_link(arg)
      # {Astrum.Worker, arg}
      {Ace.HTTP.Service,
       [
         {Astrum.Rest.Router, %{}},
         [port: 13000, cleartext: true, max_line_length: 100_000_000]
       ]},
      {Kora.Server, [port: 12000]},
      {Kora.Scheduler, []},
      {Astrum.Server, 16000}
    ]

    # See https://hexdocs.pm/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: Astrum.Supervisor]
    Supervisor.start_link(children, opts)
  end
end
