require Logger

defmodule Mix.Tasks.Astrum.Iso do
  use Mix.Task

  def run(_) do
    File.ls!("/home/dax/tmp/compressed")
    |> Hull.Time.Progress.print()
    |> Task.async_stream(fn file -> upload(file) end,
      ordered: false,
      max_concurrency: 50,
      timeout: :timer.hours(1)
    )
    |> Stream.run()
  end

  def upload(file) do
    {x, y, direction} = parse(file)
    key = Astrum.Plot.key_encode(x, y)

    {:ok, token} = Gondola.for_scope("https://www.googleapis.com/auth/cloud-platform")

    conn = GoogleApi.Storage.V1.Connection.new(token.access_token)

    path = "iso/#{key}#{direction}.jpg"

    {:ok, _} =
      GoogleApi.Storage.V1.Api.Objects.storage_objects_insert_simple(
        conn,
        "public.exploreastrum.com",
        "multipart",
        %{name: path, contentType: "image/jpeg"},
        "/home/dax/tmp/compressed/" <> file
      )
  end

  def parse(input) do
    [_, x] = Regex.run(~r/x(\d{1,2})/, input)
    [_, y] = Regex.run(~r/y(\d{1,2})/, input)

    position =
      cond do
        String.contains?(input, "rightbackward") -> "nw"
        String.contains?(input, "rightforward") -> "sw"
        String.contains?(input, "leftbackward") -> "ne"
        String.contains?(input, "leftforward") -> "se"
      end

    {63 - String.to_integer(x) + 64, 63 - String.to_integer(y) + 64, position}
  end

  def parse_position("rightbackward" <> _), do: "nw"
  def parse_position("rightforward" <> _), do: "sw"
  def parse_position("leftbackward" <> _), do: "ne"
  def parse_position("leftforward" <> _), do: "se"
end
