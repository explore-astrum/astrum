import Kora from './kora'
import * as User from './user'

export function user() {
	return Kora.local_path<string>(['session', 'user'])
}

export function data() {
	return User.data(user())
}