import { Kora, MemoryStore } from '@ironbay/kora'
import KoraRouter from 'kora-router'

const kora = new Kora(new MemoryStore())
kora.connect('ws://localhost:12000/socket')
const router = new KoraRouter(kora)

export { kora, router }

