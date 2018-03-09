import * as React from 'react'
import * as ReactDOM from 'react-dom'
import wrap from '../wrap'
import { Input } from './index'

import { Container } from '../container'
import { Icon } from '../image'

interface Props {
	value: string
	options: { [key: string]: any }
	onChange: (input: string) => void
	disabled?: boolean
	[key: string]: any
}

interface State {
	focus: boolean,
	filter: string
}

export default class Autocomplete extends React.Component<Props, State> {
	constructor(props: Props) {
		super(props)
		this.state = {
			focus: false,
			filter: '',
		}
	}
	render() {
		const { value, options, onChange, disabled, ...rest } = this.props
		const { focus, filter } = this.state
		const matches = this.matches(options, filter)
		const display = options[value]
		return (
			<Container onBlur={this.handle_focus.bind(this, false)} >
				<Container align-center flex>
					{
						display && 
							<Container align-center mgn-r4 flex>
								<Container size-4 weight-5 className="input" mgn-r2>{display}</Container>
								{!disabled && <Icon src="x" onClick={() => this.handle_change(undefined)} />}
							</Container>
					}
					{
						!disabled &&
							<Container flex-grow>
								<Input
									value={this.state.filter}
									onChange={this.handle_search}
									onFocus={this.handle_focus.bind(this, true)}
									{...rest} />
							</Container>
					}
				</Container>
				<Container
					style={{
						position: 'absolute',
						top: '100%',
						right: '0px',
						left: '0px',
					}}
					bg-light-gray
					hide={!focus || !filter}
					>
					{
						matches.map(([k, v]) => {
							return (
								<Container cursor-pointer pad-h5 pad-v5 key={k} onMouseDown={() => this.handle_change(k)} >
									<Container size-3-5>{v}</Container>
								</Container>
							)
						})
					}
					{
						matches.length === 0 && 
								<Container cursor-pointer pad-h5 pad-v5 >
									<Container size-3-5>No matches found</Container>
								</Container>
					}
				</Container>
			</Container>
		)
	}
	private handle_change = value => {
		this.props.onChange(value)
		this.setState({
			filter: '',
		})
	}
	private handle_focus = value => {
		this.setState({
			focus: value,
			filter: '',
		})
	}
	private handle_search = e => {
		this.setState({
			filter: e.target.value
		})
	}
	private matches(options: Object, filter: string) {
		if (!filter)
			return []
		filter = filter.toLowerCase()
		return Object
		.entries(options)
		.filter(([_, value]) => (value || '').toLowerCase().indexOf(filter) > -1)
	}
}