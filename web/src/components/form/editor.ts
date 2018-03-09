import { Dynamic } from '@ironbay/kora'

export default class Editor<T> {
	public changes: Mutation
	private _component: React.Component
	private _path: string[]

	constructor(component: React.Component, path: string[]) {
		this._component = component
		this._path = path
		this.changes = {
			merge: {},
			delete: {},
		}
	}

	public handle(path: string[], cb = (value: any) => {}) {
		return (e: React.ChangeEvent<HTMLInputElement | HTMLSelectElement | HTMLTextAreaElement>) => {
			const { value } = e.target
			this.merge(path, value, cb)
		}
	}

	public handle_json(path: string[], cb = (value: any) => {}) {
		return (e: React.ChangeEvent<HTMLInputElement | HTMLSelectElement | HTMLTextAreaElement>) => {
			const { value } = e.target
			this.merge(path, JSON.parse(value), cb)
		}
	}

	public edit(item: T) {
		this.changes = {
			merge: {},
			delete: {},
		}
		this._component.setState(state => Dynamic.put(state, this._path, JSON.parse(JSON.stringify(item))))
	}

	public merge(path: string[], value: any, cb = (v: any) => {}) {
		const full = [...this._path, ...path]
		Dynamic.put(this.changes.merge, path, value)
		this._component.setState(state => {
			return Dynamic.put(state, full, value)
		}, () => cb(value))
	}

	public delete(path: string[], cb = () => {}) {
		const full = [...this._path, ...path]
		Dynamic.put(this.changes.delete, path, 1)
		this._component.setState(state => {
			return Dynamic.delete(state, full)
		}, () => cb())
	}
}