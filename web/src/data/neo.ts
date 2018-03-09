import * as Session from './session'
import Kora from './kora'
import Neon, { api, rpc, sc, wallet, CONST, u, tx } from '@cityofzion/neon-js'

const SCRIPT = '2739bd1bcfa1b0368962252e189a683817c9da8e'

const client = new rpc.RPCClient(CONST.DEFAULT_RPC.TEST, '2.3.3')

export async function plot_transfer(x, y, to) {
	const result = await invoke('plot.transfer', [x, y, to])
	return await transaction('plot.transfer', [x, y, to])
}

export async function relic_buy(owner, relic, amount) {
	const a = account()
	const result = await transaction('relic.buy', [owner, a.address, relic, amount])
	return result
}

export function account() {
	return new wallet.Account(Session.data().wif)
}

async function invoke(operation: string, args: any[]) {
	const i = create_invoke(operation, args)
	console.dir(i)
	const result: any = await client.invokeScript(i)
	if (result.stack.length === 0) {
		console.dir(result)
		throw 'Invocation failed'
	}
	return result.stack[0]
}

async function transaction(operation: string, args: any[]) {
	const a = account()
	const balance = await Neon.get.balance('TestNet', a.address)
	const unsigned = tx.Transaction.createInvocationTx(balance, api.makeIntent({GAS: 0.00000001}, a.address), create_invoke(operation, args), 0, { version: 1})
	const signed = Neon.sign.transaction(unsigned, a.privateKey)
	const result = await client.sendRawTransaction(signed)
	if (!result)
		throw 'Transaction failed'
	return signed.hash as any
}

function create_invoke(operation: string, args: any[]) {
	const a =
		args
		.map(item => {
			if (typeof item === 'number')
				return sc.ContractParam.integer(item)
			if (typeof item === 'string' && item.length === 34)
				return sc.ContractParam.byteArray(item, 'address')
			if (typeof item === 'string')
				return sc.ContractParam.string(item)
		})
	console.dir(a)
	return Neon.create.script({
		scriptHash: SCRIPT,
		operation: operation,
		args: a
	})
}