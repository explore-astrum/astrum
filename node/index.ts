import Neon, { api, rpc, sc, wallet, CONST, u, tx } from '@cityofzion/neon-js'
import * as http from 'http'

const root = new wallet.Account(process.env.ROOT_KEY || '')

let balance: any = null;

async function send(to: string, amount: number) {
	const to_account = new wallet.Account(to)
	const balance = await Neon.get.balance('TestNet', root.address)
	console.dir(balance)
	const intent = api.makeIntent({ GAS: amount }, to_account.address)
	const unsigned = tx.Transaction.createContractTx(balance, intent, {})
	const signed = Neon.sign.transaction(unsigned, root.privateKey)
	const { result } = await rpc.Query.sendRawTransaction(signed).execute(CONST.DEFAULT_RPC.TEST)
	console.log('Sent', amount, 'to', to, result)
	if (result)
		balance.applyTx(signed)
	return result
}

const server = http.createServer(async (req, resp) => {
	try {
		if (!req.url)
			return
		const matches = /bootstrap\/(.+)$/g.exec(req.url)
		if (!matches)
			return
		const address = matches[1]
		const result = await send(address, 0.00000001)
		resp.end(JSON.stringify(result))
	}
	catch (ex) {
		console.dir(ex)
		resp.end("false")
	}
})
server.listen(8000)
