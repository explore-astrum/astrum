import * as React from 'react'
import * as cs from 'classnames'
import legos from '@ironbay/legos/dist/classes'

type Extra = {
	[key: string]: any
}

export function component<T>(element: any, base_class: string, forced_props = {}): (props: T & Extra) => React.SFCElement<T & Extra> {
	return function(props: T & Extra) {
		const classes = {}
		let { children, className, ...rest} = props as any
		rest = {
			...forced_props,
			...rest,
		}
		for (let key of Object.keys(rest)) {
			if (legos[key]) {
				classes[key] = rest[key]
				delete rest[key]
			}
		}
		const combined = cs(base_class, className, classes)
		return React.createElement<T & Extra>(element, {
			className: combined,
			...rest,
		}, children)
	}
}
