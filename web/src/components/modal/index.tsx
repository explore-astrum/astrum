import './styles.css'
import * as React from 'react'
import { component } from '../theme'
import { Container } from '../container'
import { Icon } from '../image'

interface Props {
	active: boolean,
	slider?: boolean,
	onHide?: () => void,
	children?: any
}

interface State {

}

export function Root(props: Props) {
	return (
		<Container
			onClick={props.onHide}
			style={{
				background: 'rgba(0, 0, 0, .25)',
				transition: '300ms all',
				opacity: props.active ? 100 : 0,
				// zIndex: props.active ? 10000 : -10000,
				position: 'fixed',
				left: 0,
				right: 0,
				top: 0,
				bottom: 0,
				pointerEvents: props.active ? 'all' : 'none',
			}}
			pad-4={!props.slider}
			flex
			justify-center={!props.slider}
			align-center={!props.slider}
			justify-end={props.slider}
			cursor-pointer>
			<Container
				flex={props.slider}
				flex-column={props.slider}
				onClick={e => e.stopPropagation()}
				style={{
					flexBasis: '30rem',
					transition: '300ms all',
					cursor: 'initial',
					overflowY: props.slider ? 'initial' : 'auto',
					maxHeight: props.slider ? '100%' : '80%',
					transform:
						!props.active && !props.slider ? 'translate3d(0, 50px, 0)' :
						!props.active && props.slider ? 'translate3d(25%, 0, 0)' :
						'translate3d(0, 0, 0)'
				}}
				bg-white>
				{props.children}
			</Container>
		</Container>
	)
}

export const Title = component<React.HTMLAttributes<HTMLDivElement>>(Container, '', {
	'bg-light-gray': true,
	'pad-v5': true,
	'weight-5': true,
	'fg-gray': true,
	'text-center': true,
})

export const Footer = component<React.HTMLAttributes<HTMLDivElement>>(Container, '', {
	'bg-dark-gray': true,
	'fg-white': true,
	'uppercase': true,
	'size-3': true,
	'cursor-pointer': true,
	'pad-v5': true,
	'weight-5': true,
	'text-center': true,
})
