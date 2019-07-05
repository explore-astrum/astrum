defmodule Astrum.Rest.Router do
  use Raxx.Router

  section(
    [],
    [
      {%{path: ["rest", "stripe" | _rest]}, Astrum.Rest.Stripe}
    ]
  )
end
