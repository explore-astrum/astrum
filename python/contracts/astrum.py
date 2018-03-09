from boa.blockchain.vm.Neo.Storage import GetContext, Get, Put, Delete
from boa.blockchain.vm.Neo.Action import RegisterAction
from boa.blockchain.vm.Neo.Runtime import Log, GetTrigger, CheckWitness, Notify
from boa.code.builtins import concat

def Main(op, args):

    """
    Parse operation
    """
    context = GetContext()

    if op == 'owner' and len(args) == 0:
        return owner(context)
    if op == 'owner.set' and len(args) == 1:
        to = args[0]
        return set_owner(context, to)

    if op == 'plot.owner.key' and len(args) == 2:
        x = args[0]
        y = args[1]
        return plot_owner_key(x, y)

    if op == 'plot.owner' and len(args) == 2:
        x = args[0]
        y = args[1]
        return plot_owner(context, x, y)

    if op == 'plot.transfer' and len(args) == 3:
        x = args[0]
        y = args[1]
        to = args[2]
        return plot_transfer(context, x, y, to)

    if op == 'relic.balance' and len(args) == 2:
        owner = args[0]
        relic = args[1]
        return relic_balance(context, owner, relic)

    if op == 'relic.transfer' and len(args) == 4:
        owner = args[0]
        to = args[1]
        relic = args[2]
        amount = args[3]
        return relic_transfer(context, owner, to, relic, amount)

    if op == 'relic.create' and len(args) == 3:
        to = args[0]
        relic = args[1]
        amount = args[2]
        return relic_create(context, to, relic, amount)

    if op == 'relic.sell' and len(args) == 4:
        owner = args[0]
        relic = args[1]
        amount = args[2]
        price = args[3]
        return relic_sell(context, owner, relic, amount, price)

    if op == 'relic.buy' and len(args) == 4:
        owner = args[0]
        to = args[1]
        relic = args[2]
        amount = args[3]
        return relic_buy(context, owner, to, relic, amount)
    
    return "Invalid command + args"

def safe_get(context, key, fallback):
    result = Get(context, key)
    if not result:
        return fallback
    return result

def default(input, fallback):
    if input is None:
        return fallback
    return input

"""
Owner
"""
def owner(context):
    return Get(context, "owner")

def set_owner(context, to):
    previous = owner(context)
    if not previous:
        previous = to
    if not CheckWitness(previous):
        return "Only current owner can transfer ownership"
    Put(context, "owner", to)
    return True


"""
Plots
"""
OnPlotTransfer = RegisterAction('plot.transfer', 'owner', 'to', 'x', 'y')

def plot_owner_key(x, y):
    return 'po:' + x + ':' + y

def plot_sale_key(x, y):
    return 'ps:' + x + ':' + y

def plot_owner(context, x, y):
    key = plot_owner_key(x, y)
    return Get(context, key)

def plot_transfer(context, x, y, to):
    key = plot_owner_key(x, y)
    owner = plot_owner(context, x, y)
    if owner and not CheckWitness(owner):
        return "You are not allowed to transfer this plot"
    Put(context, key, to)
    OnPlotTransfer(owner, to, x, y)
    return True

"""
Relics
"""

OnRelicBalance = RegisterAction('relic.balance', 'owner', 'relic', 'amount')
OnRelicSell = RegisterAction('relic.sell', 'owner', 'relic', 'amount')
OnRelicPrice = RegisterAction('relic.price', 'owner', 'relic', 'price')
OnRelicTransfer = RegisterAction('relic.transfer', 'owner', 'to', 'relic', 'price')

def relic_total_key(relic):
    return 'rt:' + relic

def relic_balance_key(owner, relic):
    return 'rb:' + owner + ":" + relic

def relic_sale_key(owner, relic):
    return 'rs:' + owner + ":" + relic

def relic_price_key(owner, relic):
    return 'rp:' + owner + ":" + relic

def relic_total(context, relic):
    key = relic_total_key(relic)
    return Get(context, key)

def relic_balance(context, owner, relic):
    key = relic_balance_key(owner, relic)
    return safe_get(context, key, 0)

def relic_sale(context, owner, relic):
    key = relic_sale_key(owner, relic)
    return safe_get(context, key, 0)

def relic_price(context, owner, relic):
    key = relic_price_key(owner, relic)
    return safe_get(context, key, 0)

def relic_create(context, to, relic, amount):
    if amount <= 0:
        return "Please specify valid amount" 
    owner = owner(context)
    if not CheckWitness(owner):
        return "Only contract owner can create relics"
    if relic_total(context, relic) > 0:
        return "This relic has already been created"
    total_key = relic_total_key(relic)
    Put(context, total_key, amount)
    relic_balance_write(context, to, relic, amount)
    return True

def relic_transfer(context, owner, to, relic, amount):
    if not CheckWitness(owner):
        return "You are not allowed to transfer this balance"
    owner_balance = relic_balance(context, owner, relic) - amount
    if owner_balance < 0:
        return "Amount exceeds balance"
    to_balance = relic_balance(context, to, relic) + amount

    relic_balance_write(context, owner, relic, owner_balance)
    relic_balance_write(context, to, relic, to_balance)

    sale_balance = relic_sale(context, owner, relic)
    if sale_balance > owner_balance:
        relic_sell_write(context, owner, relic, owner_balance)
    OnRelicTransfer(owner, to, relic, amount)
    return True

def relic_balance_write(context, owner, relic, amount):
    owner_key = relic_balance_key(owner, relic)
    Put(context, owner_key, amount)
    OnRelicBalance(owner, relic, amount)

def relic_sell(context, owner, relic, amount, price):
    if not CheckWitness(owner):
        return "You are not allowed to set this sell balance"
    balance = relic_balance(context, owner, relic)
    if balance < amount:
        return "Amount exceeds balance"
    relic_sell_write(context, owner, relic, amount)
    relic_price_write(context, owner, relic, price)
    return True

def relic_buy(context, owner, to, relic, amount):
    if not CheckWitness(to):
        return "You are not allowed to purchase this relic"
    price = relic_price(context, owner, relic)
    if price > 0:
        return "This relic is not free"
    to_balance = relic_balance(context, to, relic)
    if to_balance > 0:
        return "You already have this relic"
    sale = relic_sale(context, owner, relic)
    if amount > sale:
        return "Not enough for sale"
    to_balance = to_balance + amount
    sale_balance = sale - amount
    owner_balance = relic_balance(context, owner, relic) - amount
    relic_balance_write(context, owner, relic, owner_balance)
    relic_balance_write(context, to, relic, to_balance)
    relic_sell_write(context, owner, relic, sale_balance)
    return True

def relic_sell_write(context, owner, relic, amount):
    sale_key = relic_sale_key(owner, relic)
    Put(context, sale_key, amount)
    OnRelicSell(owner, relic, amount)

def relic_price_write(context, owner, relic, price):
    price_key = relic_price_key(owner, relic)
    Put(context, price_key, price)
    OnRelicPrice(owner, relic, price)