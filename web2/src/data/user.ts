import { kora } from './kora'

export function info(key: string) {
    return schema(kora.local_path<UserInfo>(['user:info', key]))
}

export function schema(user: UserInfo) {
    user = user || {}
    return user
}