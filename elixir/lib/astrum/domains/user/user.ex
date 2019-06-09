defmodule Astrum.User do
  require Dynamic

  Dynamic.schema(%{
    "key" => nil,
    "contact" => %{
      "email" => nil
    }
  })

  def uuid() do
    "USR" <> Kora.UUID.ascending()
  end
end
