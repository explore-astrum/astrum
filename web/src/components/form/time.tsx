import * as React from 'react'
import * as moment from 'moment'

import wrap from '../wrap'
import { Container } from '../container'
import * as Form from './index'
import * as Validators from './validators'

interface Props {
	value: number,
	onChange: (input: number) => void,
	[key: string]: any
}

const RANGE = 5
const DISPLAY_FORMAT = 'hh:mma'

export default class Time extends React.Component<Props, any> {
	private editing: any
	constructor(props) {
		super(props)
		this.state = {
			input: false,
		}
	}
	render() {
		const { value, onChange, ...rest } = this.props
		const ts = moment(value)
		const { input } = this.state
		return (
			<Form.Input
				onBlur={this.set_time}
				fg-red={input && !Validators.time(input)}
				onChange={this.handle_change}
				value={input === false ? ts.format(DISPLAY_FORMAT) : input}
				{...rest}
			/>
		)
	}
	private handle_change = e => {
		const { value } = e.target
		this.setState({
			input: value
		})
	}
	private set_time = () => {
		const parsed = moment(this.state.input, DISPLAY_FORMAT)
		const value = moment(this.props.value)
		const next = parsed.isValid() ? parsed : value
		this.props.onChange(
			value
			.set({
				hour: next.hour(),
				minute: next.minute(),
				second: next.second(),
			})
			.toDate()
			.getTime()
		)
		this.setState({
			input: false
		})
	}
}
