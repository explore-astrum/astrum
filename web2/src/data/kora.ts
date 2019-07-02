import { Kora, MemoryStore } from '@ironbay/kora'
import KoraRouter from '@ironbay/kora-router'

const kora = new Kora(new MemoryStore())
kora.connect('ws://localhost:12000/socket')
const router = new KoraRouter(kora)
kora.onLocalChange.add(() => console.dir(kora.local_path([])))

kora.before_mutation(['plot:info', '+'], (path, mut) => {
    const { owner } = mut.merge
    if (!owner) return false
    kora.query_path(['user:info', owner])
})

kora.before_mutation(['connection'], async (_path, mut) => {
    if (mut.merge.status !== 'ready') return
})

export { kora, router }
