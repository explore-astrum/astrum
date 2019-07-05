import { Kora, MemoryStore } from '@ironbay/kora'
import KoraRouter from '@ironbay/kora-router'
import * as User from './user'

const kora = new Kora(new MemoryStore())
kora.settings.batch = true
kora.connect('ws://localhost:12000/socket')
const router = new KoraRouter(kora)
// kora.onLocalChange.add(() => console.dir(kora.local_path([])))

kora.before_mutation(['connection'], async (_path, mut) => {
    if (mut.merge.status !== 'ready') return
    kora.send('kora.subscribe', 1, 0)
})

kora.before_mutation(['session'], async (_path, mut) => {
    const user = mut.merge.user
    if (!user) return
    kora.query({
        'user:info': {
            [user]: {}
        }
    })
})

kora.before_mutation(['plot:activity', '+', '+'], async (_path, mut) => {
    const sender = mut.merge.sender
    if (!sender || User.info(sender).key) return
    await kora.query_path(['user:info', sender])
})

export { kora, router }
