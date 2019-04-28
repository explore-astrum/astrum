defmodule Astrum.Packet do
  def decode(
        <<0::unsigned-integer-size(16), type::unsigned-integer-size(32), relic::binary-size(16)>>
      ),
      do: {:relic_create, {type, relic}}

  def decode(<<1::unsigned-integer-size(16), relic::binary-size(16), owner::binary-size(20)>>),
    do: {:relic_owner, {relic, owner}}

  def decode(
        <<2::unsigned-integer-size(16), relic::binary-size(16), x::float-size(32),
          y::float-size(32), z::float-size(32)>>
      ),
      do: {:relic_position, {relic, {x, y, z}}}

  def decode(<<3::unsigned-integer-size(16)>>), do: {:ping, {}}
  def decode(<<4::unsigned-integer-size(16)>>), do: {:pong, {}}

  def decode(_), do: {:error, :unknown}

  def relic_create(type, relic),
    do: <<0::unsigned-integer-size(16), type::unsigned-integer-size(32), relic::binary-size(16)>>

  def relic_owner(relic, owner),
    do: <<1::unsigned-integer-size(16), relic::binary-size(16), owner::binary-size(20)>>

  def relic_position(relic, x, y, z),
    do:
      <<2::unsigned-integer-size(16), relic::binary-size(16), x::float-size(32),
        y::float-size(32), z::float-size(32)>>

  def ping(), do: <<3::unsigned-integer-size(16)>>
  def pong(), do: <<4::unsigned-integer-size(16)>>

  def plot_create(x, y, plot),
    do: <<
      5::unsigned-integer-size(16),
      x::signed-integer-size(16),
      y::signed-integer-size(16),
      plot::binary-size(8)
    >>

  def plot_owner(plot, owner),
    do: <<
      6::unsigned-integer-size(16),
      plot::binary-size(8),
      owner::binary-size(20)
    >>
end
