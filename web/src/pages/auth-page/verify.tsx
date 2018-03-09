import * as React from 'react'
import { Route, Switch } from 'react-router-dom'
import { wallet } from '@cityofzion/neon-js'

import Kora from '../../data/kora'
import { Grid, Container, Wrap } from '../../components/container'
import * as Form from '../../components/form'

import { Title, Body } from './components'
import Header from '../../structures/header'

export default class Verify extends React.Component<any, any> {
	constructor(props) {
		super(props)
		this.state = {}
	}
	async componentWillMount() {
		const { token } = this.props.match.params
		if (!token)
			return
		const account = new wallet.Account(wallet.generatePrivateKey())
		const auth = await Kora.send<string>('auth.login', {
			token,
			address: account.address,
			wif: account.WIF,
		}, 1)
		localStorage.setItem('token', auth)
		await Kora.local_merge(['connection', 'status'], 'ready')
		this.props.history.push('/marketplace')
	}
	render() {
		const { form } = this.state
		const { token } = this.props.match.params
		return (
			<Container>
				<Header />
				<Wrap>
					<Container flex justify-center mgn-t8 pad-t8>
						<Container flex-5>
						{
							!token &&
							<Container>
								<Title>Verify Your Email</Title>
								<Body>
									We sent you an email. Click the link in the email to verify your address and access Astrum
								</Body>
							</Container>
						}
						{
							token &&
							<Container>
								<Title>Setting up your account...</Title>
								<Body>
									Hang on a moment, we're creating and seeding your wallet
								</Body>
							</Container>
						}
						</Container>
					</Container>
				</Wrap>
			</Container>
		)
	}
}