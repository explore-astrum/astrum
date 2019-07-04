import { kora } from './kora'

export function token() {
    return localStorage.getItem('token')
}

export function user() {
    return kora.local_path<string | false>(['session', 'user'])
}

export async function upgrade(token: string) {
    const user = await kora.send<string>('auth.upgrade', token, 1)
    localStorage.setItem('token', token)
    await kora.local_merge(['session', 'user'], user)
}

let locked = false
export async function lock(cb: () => void) {
    if (locked) return
    locked = true
    try {
        await cb()
    } catch (ex) {
        alert(ex)
    }
    locked = false
}

const existing = token()
if (existing) upgrade(existing).catch(() => kora.local_merge(['session', 'user'], false))
if (!existing) kora.local_merge(['session', 'user'], false)
