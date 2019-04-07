defmodule Astrum.Packet do
  def decode(
        <<0::unsigned-integer-size(16), type::unsigned-integer-size(32), relic::binary-size(16)>>
      ),
      do: {:relic_create, {type, relic}}

  def decode(<<1::unsigned-integer-size(16), relic::binary-size(16), owner::binary-size(20)>>),
    do: {:relic_owner, {relic, owner}}

  def decode(
        <<2::unsigned-integer-size(16), relic::binary-size(16), x::float, y::float, z::float>>
      ),
      do: {:relic_position, {relic, {x, y, z}}}

  def decode(_), do: {:error, :unknown}

  def encode({:relic_create, {type, relic}}),
    do: <<0::unsigned-integer-size(16), type::unsigned-integer-size(32), relic::binary-size(16)>>

  def encode({:relic_owner, {relic, owner}}),
    do: <<1::unsigned-integer-size(16), relic::binary-size(16), owner::binary-size(20)>>

  def encode({:relic_position, {relic, {x, y, z}}}),
    do: <<2::unsigned-integer-size(16), relic::binary-size(16), x::float, y::float, z::float>>

  def encode(_), do: {:error, :unknown}
end
