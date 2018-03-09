defmodule Astrum.MixProject do
  use Mix.Project

  def project do
    [
      app: :astrum,
      version: "0.1.0",
      elixir: ">= 1.5.2",
      start_permanent: Mix.env() == :prod,
      deps: deps()
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger],
      mod: {Astrum.Application, []}
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:kora, github: "ironbay/kora"},
      {:dynamic_ex, github: "ironbay/dynamic_ex"},
      {:fig, github: "ironbay/fig"},
      {:swoosh, "~> 0.11.0"},
      {:cowboy, "~> 2.2", override: true},
			{:plug, "~> 1.5.0-rc.0", override: true},
      {:httpoison, "~> 0.13"},
      # {:dep_from_hexpm, "~> 0.3.0"},
      # {:dep_from_git, git: "https://github.com/elixir-lang/my_dep.git", tag: "0.1.0"},
    ]
  end
end
