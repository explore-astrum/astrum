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
      {:hull, github: "ironbay/hull"},
      {:kora, github: "ironbay/kora"},
      {:elmdb, github: "ironbay/elmdb"},
      {:gondola, github: "ironbay/gondola"},
      {:google_api_storage, "~> 0.1.0"},
      {:poison, "~> 3.1.0", override: true}
    ]
  end
end
