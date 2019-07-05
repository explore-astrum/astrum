defmodule Astrum.Ledger do
  require Dynamic
  use Kora.Interceptor

  Dynamic.schema(%{
    "key" => nil,
    "created" => nil,
    "amount" => nil,
    "where" => nil,
    "data" => nil
  })

  def all(target) do
    ["ledger", target]
    |> Kora.query_path!()
    |> Dynamic.default(%{})
  end

  def sum(target) do
    target
    |> all()
    |> Stream.map(fn {_, %{"amount" => amount}} -> amount end)
    |> Enum.sum()
  end

  def uuid() do
    "TXN" <> Kora.UUID.descending()
  end

  def create(ledger, amount, where, data) do
    create(uuid(), ledger, amount, where, data)
  end

  def create(key, ledger, amount, where, data) do
    Kora.Mutation.merge(["ledger", ledger, key], %{
      "key" => key,
      "amount" => amount,
      "where" => where,
      "data" => data
    })
  end

  def transfer(from, to, amount, data \\ %{}) do
    key = uuid()

    Kora.Mutation.combine([
      create(key, from, amount * -1, to, data),
      create(key, to, amount, from, data)
    ])
  end

  def before_mutation(
        ["ledger", target, key],
        %{
          merge:
            txn = %{
              "key" => key,
              "amount" => amount,
              "where" => where
            }
        },
        _mut,
        _user
      )
      when amount < 0 do
    target
    |> sum()
    |> case do
      result when abs(amount) > result ->
        {:error, :ledger_low}

      _ ->
        {:combine, create(key, where, amount * -1, target, data(txn))}
    end
  end
end
