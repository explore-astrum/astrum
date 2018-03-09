defmodule AstrumTest do
  use ExUnit.Case
  doctest Astrum

  test "greets the world" do
    assert Astrum.hello() == :world
  end
end
