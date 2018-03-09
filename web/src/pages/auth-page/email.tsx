import * as React from 'react'
import { Route, Switch } from 'react-router-dom'
import { wallet } from '@cityofzion/neon-js'

import Kora from '../../data/kora'
import { Grid, Container, Wrap } from '../../components/container'
import * as Form from '../../components/form'
import Header from '../../structures/header'

import { Title, Body } from './components'

export default class Email extends React.Component<any, any> {
	private editor: Form.Editor<any> = new Form.Editor<any>(this, ['form'])
	constructor(props) {
		super(props)
		this.state = {
			form: {
				email: ''
			}
		}
	}
	async componentWillMount() {
	}
	render() {
		return (
			<Container>
				<Header />
				<Wrap>
					<Container flex justify-center mgn-t8 pad-t8>
						<Container flex-5>
							<Container>
								<Title>Welcome to Astrum</Title>
								<Body>Enter your email below and we'll send a magic link to your inbox so you can access your account</Body>
								<Container mgn-t8>
									<Form.Label>Email Address</Form.Label>
									<Container pad-4 border-1 radius-4>
										<Form.Input
											onChange={this.editor.handle(['email'])}
											value={this.state.form.email || ''}
											placeholder="Email Address" />
									</Container>
									<Container onClick={() => this.handle_submit()} bg-dark-gray fg-white size-3 weight-6 cursor-pointer text-center pad-4 radius-4 uppercase mgn-t4>Login</Container>
								</Container>
							</Container>
						</Container>
					</Container>
				</Wrap>
			</Container>
		)
	}
	private async handle_submit() {
		const { email } = this.state.form
		await Kora.send('auth.email', { email }, 1)
		this.props.history.push('/auth/verify')
	}
}