import { Container } from '../../components/container'
import { component } from '../../components/theme'

export const Title = component<React.HTMLAttributes<HTMLDivElement>>(Container, '', {
	'text-center': true,
	'size-8': true,
	'pad-v2': true
})

export const Body = component<React.HTMLAttributes<HTMLDivElement>>(Container, '', {
	'text-center': true,
	'fg-gray': true,
	'line-7': true,
	'size-4': true,
})

export const Button = component<React.HTMLAttributes<HTMLDivElement>>(Container, '', {
	'bg-blue': true,
	'fg-white': true,
	'text-center': true,
	'weight-5': true,
	'pad-v3': true,
	'radius-3': true,
	'cursor-pointer': true,
	'unselectable': true,
})