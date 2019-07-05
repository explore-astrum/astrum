defmodule Astrum.MixProject do
  use Mix.Project

  def project do
    [
      app: :astrum,
      version: "0.1.0",
      elixir: "~> 1.8",
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
      {:fig, github: "ironbay/fig"},
      {:stripity_stripe, "~> 2.2.0"},
      {:hull, github: "ironbay/hull"},
      # {:kora, github: "ironbay/kora"},
      {:kora, path: "/home/dax/dev/projects/kora/kora"},
      {:gondola, github: "ironbay/gondola"},
      {:google_api_storage, "~> 0.1.0"},
      {:poison, "~> 3.1.0", override: true},
      {:joken, "~> 2.0"},
      {:comeonin, "~> 3.0"},
      {:raxx, "~> 0.18.1"},
      {:ace, "~> 0.18.1"},
      {:distillery, "~> 2.0"}
    ]
  end
end
