import * as React from 'react'
import * as moment from 'moment'

import wrap from '../wrap'
import { Container, Grid } from '../container'
import { Icon } from '../image'
import * as Form from './index'

interface Props {
	value: number,
	onChange: (value: number) => void,
	[key: string]: any
}

const RANGE = 5
const FORMAT = 'M/D/YYYY'
const DISPLAY_FORMAT = 'MM/DD/YYYY'

export default class Date extends React.Component<Props, any> {
	constructor(props, context) {
		super(props, context)
		this.state = {
			value: false,
			focus: false,
		}
	}
	render() {
		const { focus } = this.state
		const { value, onChange, disabled, ...rest } = this.props
		const ts = moment(value)
		return (
			<Container flex-column >
				<Container flex>
					<Container flex-grow>
						<Form.Input
							fg-red={this.state.value}
							value={this.state.value === false ? ts.format(DISPLAY_FORMAT) : this.state.value}
							onChange={e => this.handle_change(e.target.value)}
							disabled={disabled}
							{...rest} />
					</Container>
					{!disabled && !focus && <Icon src="calendar" onClick={() => this.handle_focus(!focus)} />}
					{!disabled && focus && <Icon src="chevron-down" onClick={() => this.handle_focus(!focus)} />}
				</Container>
				{focus && this.render_dropdown(ts)}
			</Container>
		)
	}
	render_dropdown(ts: moment.Moment) {
		const start = ts.clone().startOf('month')
		const end = ts.clone().endOf('month')
		const days = [] as Array<moment.Moment>
		for (let m = start.clone().startOf('week'); m.isBefore(end); m.add('days', 1)) {
			days.push(m.clone())
		}
		return (
			<Container
				style={{
					position: 'absolute',
					top: '100%',
					right: '0px',
					left: '0px',
				}}
				flex
				flex-column
				bg-light-gray>
				<Container flex justify-center align-center>
					<Arrow flip={false} onClick={() => this.handle_date(start.clone().add(-1, 'month'))} />
					<Container justify-center flex-grow text-center pad-v6 weight-5>{start.format('MMMM YYYY')}</Container>
					<Arrow flip={true} onClick={() => this.handle_date(start.clone().add(1, 'month'))} />
				</Container>
				<Grid columns={7} flush pad-2>
					{
						days.map((item, n) => {
							const disabled = item.month() !== ts.month()
							const active = !disabled && item.date() === ts.date()
							return (
								<Container
									key={n}
									flex-grow
									pad-v2
									cursor-pointer={!disabled}
									onClick={() => {
										if (disabled)
											return
										this.handle_date(item, true)
										this.handle_focus(false)
									}}
								>
									<Container
										text-center
										mgn-b1
										hide={disabled}
										weight-6={active}
										fg-blue={active}>
										{item.format('DD')}
									</Container>
									<Container
										text-center
										hide={disabled}
										size-3
										weight-3
										fg-blue={active}
										weight-6={active}>{item.format('ddd')}
									</Container>
								</Container>
							)
						})
					}
				</Grid>
			</Container>
		)
	}
	handle_date = (value: moment.Moment, hide = false) => {
		this.handle_change(value.format(FORMAT))
	}
	handle_focus = value => {
		this.setState({
			focus: value,
		})
	}
	handle_change = (input: string) => {
		const { value } = this.props
		const result = this.parse(input)
		if (result) {
			this.props.onChange(this.set_date(value, result).toDate().getTime())
			this.setState({
				value: false,
			})
			return
		}
		this.setState({
			value: input
		})
	}
	private set_time(input: number, next: moment.Moment) {
		return moment(input).set({
			hour: next.hour(),
			minute: next.minute(),
			second: next.second(),
		})
	}
	private set_date(input: number, next: moment.Moment) {
		return moment(input).set({
			month: next.month(),
			date: next.date(),
			year: next.year(),
		})
	}
	private parse(input: string) {
		const format = FORMAT
		const stripped = input.replace(' ', '')
		const parsed = moment(stripped, format, true)
		if (!parsed.isValid())
			return false
		return parsed
	}
}

function Arrow({ flip, ...rest }) {
	return <Container cursor-pointer {...rest} align-center pad-h5 >{flip ? '→' : '←'}</Container>
}