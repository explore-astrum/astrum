defmodule Astrum.Rest do
	use Plug.Router
	require Logger

	plug :match 
	plug :dispatch

	def init(options) do 
		options
	end

	def child_spec(port) do
		Plug.Adapters.Cowboy2.child_spec([
			scheme: :http,
			plug: __MODULE__,
			options: [
				port: port
			]
		])
	end

	post "/rest/event" do
		{:ok, body , _} = Plug.Conn.read_body(conn)	
		body
		|> decode
		send_resp(conn, 200, "")
	end

	get "/rest/test" do
		send_resp(conn, 200, "ok")
	end

	def decode(body) do 
		body 
		|> IO.inspect
		|> Poison.decode!
		|> Dynamic.get(["message", "data"])
		|> Base.decode64!
		|> Poison.decode!
		|> process
		|> IO.inspect
	end 

	def process(["relic.balance", user, relic, amount]), do: Kora.merge!(["user:relics", user, relic, "balance"], string_to_integer(amount))
	def process(["relic.sell", user, relic, amount]), do: Kora.merge!(["user:relics", user, relic, "sale"], string_to_integer(amount))
	def process(["relic.sale", user, relic, amount]), do: Kora.merge!(["user:relics", user, relic, "sale"], string_to_integer(amount))
	def process(["relic.price", user, relic, amount]), do: Kora.merge!(["user:relics", user, relic, "price"], string_to_integer(amount))

	def process(_) do
	end

	def string_to_integer(""), do: 0
	def string_to_integer(input) when is_binary(input) do
		input
		|> Base.encode16()
		|> Integer.parse(16)
		|> case do
			{result, _} -> result
		end
	end

	def string_to_integer(input), do: input

end
