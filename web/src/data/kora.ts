import * as Constants from './constants'
import { Kora, Dynamic, MemoryStore } from '@ironbay/kora'
import Dispatcher from '@ironbay/kora/lib/dispatcher'
import * as Neo from './neo'

const kora = new Kora(Constants.URL, new MemoryStore())
kora.onLocalChange.add(() => console.dir(kora.local_path([])))

if (!localStorage.getItem('token'))
	kora.local_merge(['session', 'user'], false)

kora.before_mutation(['connection'], async (_path, mut) => {
	if (mut.merge.status !== 'ready') return
	kora.query({
		'user:relics': {
			[Constants.OWNER]: {}
		},
		'relic:info': {}
	})
	const token = localStorage.getItem('token')
	if (token) {
		try {
			const result = await kora.send<string>('auth.upgrade', token, 1)
			// await kora.send('kora.subscribe', true, 1)
			kora.local_merge(['session', 'user'], result)
			return
		} catch (ex) {
		}
	}
	kora.local_merge(['session', 'user'], false)
})

// Fetch user info on upgrade
kora.before_mutation(['session'], (_path, mut) => {
	if (!mut.merge.user) return
	const { user } = mut.merge
	kora.query({
		'user:info': {
			[user]: {},
		},
		'user:data': {
			[user]: {}
		},
		'user:transaction:pending': {
			[user]: {}
		},
		'relic:info': {},
		'user:relics': {
			[Constants.OWNER]: {}
		}
	})
})

kora.before_mutation(['user:data', '+'], (path, mut) => {
	if (!mut.merge.wif)
		return
	// Neo.plot_transfer(10, 50, mut.merge.key).then(console.dir).catch(console.dir)
})

export default kora