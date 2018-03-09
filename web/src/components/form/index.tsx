import * as React from 'react'
import * as ReactDOM from 'react-dom'
import { component } from '../theme'
import { Container } from '../container'

export { default as Editor } from './editor'
export { default as Time } from './time'
export { default as Date } from './date'
export { default as Autocomplete } from './autocomplete'
import * as validators from './validators'
export const Validators = validators

export const Input = component<React.InputHTMLAttributes<HTMLInputElement>>('input', 'input', {
	'weight-5': true,
	'size-3-5': true,
})

export const Select = component<React.InputHTMLAttributes<HTMLSelectElement>>('select', 'input', {
	'weight-5': true
})

export const TextArea = component<React.TextareaHTMLAttributes<HTMLTextAreaElement>>('textarea', 'input')

export const Block = component<React.HTMLAttributes<HTMLDivElement>>(Container, 'form-block', {
	'flex': true,
	'flex-column': true,
	'pad-4': true,
})

export const Row = component<React.HTMLAttributes<HTMLDivElement>>(Container, 'form-block', {
})

export const Label = component<React.HTMLAttributes<HTMLDivElement>>(Container, 'form-label', {
	uppercase: true,
	'size-3': true,
	'weight-6': true,
	'fg-black-40': true,
	'mgn-b2': true
})

export class Address extends React.Component<any, any> {
	private ac: google.maps.places.Autocomplete
	render() {
		const { value, onChange, ...rest } = this.props
		return <Input value={value.format} onChange={e => this.change({ format: e.target.value })} {...rest} />
	}
	componentDidMount() {
		const input = ReactDOM.findDOMNode(this) as HTMLInputElement
		const ac = new google.maps.places.Autocomplete(input)
		ac.addListener('place_changed', () => {
			const place = ac.getPlace()
			const payload = {
				format: input.value,
				raw: place.formatted_address,
				name: place.name,
				lat: place.geometry.location.lat(),
				lng: place.geometry.location.lng(),
			}
			this.change(payload)
		})
		this.ac = ac
	}
	componentWillUnmount() {
		this.ac.unbindAll()
	}
	private change(obj) {
		this.props.onChange({
			target: {
				value: obj
			}
		})
	}
}