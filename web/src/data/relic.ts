import Kora from './kora'

export interface RelicInfo {
	key: string
	name: string
	created: number
	author: string
	supply: number
	flags: {}
}

export interface UserRelic {
	key: string
	balance: number
	sale: number
	price: number
}

export function user_relics(user: string): UserRelic[] {
	return Object.entries<UserRelic>(Kora.local_path<UserRelic>(['user:relics', user]) || {}).map(([key, value]) => {
		return {
			key: key,
			...value,
		}
	})
}

export function info(relic: string) {
	return Kora.local_path<RelicInfo>(['relic:info', relic]) || {
		name: ''
	}
}

export function relic_image(relic: string) {
	return `https://storage.googleapis.com/astrum-media/relics/${relic}`
}