import Kora from './kora'
import { wallet } from '@cityofzion/neon-js'

interface UserInfo {
	key: string
	username: string
}

interface UserData {
	key: string
	email: string
	wif: string
}

export function info(key: string) {
	return Kora.local_path<UserInfo>(['user:info', key])
}

export function data(key: string) {
	return Kora.local_path<UserData>(['user:data', key])
}

export async function create(email) {
	const result = await Kora.send('auth.email', {
		email: email,
	}, 1)
}