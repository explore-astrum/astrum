import './styles.css'
import * as React from 'react'
import { Container } from '../container'
import { component } from '../theme'

export const Root = component(function({ children, ...rest}) {
	return (
		<table {...rest}>
			<tbody>
				{children}
			</tbody>
		</table>
	)
}, 'table', {})

export const Row = component('tr', 'table-row', {})

export const Header = component(function({children, ...rest}) {
	return (
		<th>
			<Container {...rest}>{children}</Container>
		</th>
	)
}, 'table-header', {
	'uppercase': true,
	'size-3': true,
	'weight-5': true,
	'fg-gray': true,
	'text-left': true,
	'pad-b2': true,
})

export const Cell = component('td', 'table-cell', {
	'pad-v3': true,
	'text-left': true,
	'style': {
		overflow: 'hidden',
		textOverflow: 'ellipsis',

	}
})
