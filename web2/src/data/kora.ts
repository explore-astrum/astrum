import { Kora, MemoryStore } from '@ironbay/kora'
import KoraRouter from '@ironbay/kora-router'

const kora = new Kora(new MemoryStore())
kora.connect('ws://localhost:12000/socket')
const router = new KoraRouter(kora)

kora.before_mutation(['plot:info', '+'], (path, mut) => {
    const { owner } = mut.merge
    if (!owner) return false
    kora.query_path(['user:info', owner])
})

export { kora, router }
