import * as React from 'react'
import { Route, Switch } from 'react-router-dom'
import { wallet } from '@cityofzion/neon-js'

import Kora from '../../data/kora'
import { Grid, Container, Wrap } from '../../components/container'
import * as Form from '../../components/form'

import { Title, Body } from '../auth-page/components'
import Header from '../../structures/header'

export default class Land extends React.Component<any, any> {
	constructor(props) {
		super(props)
		this.state = {}
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
								<Title>Hang in there!</Title>
								<Body>
									We're still working on perfecting every inch of Astrum.  We'll keep you updated via email so you'll know as soon as our land sale goes live
								</Body>
							</Container>
						</Container>
					</Container>
				</Wrap>
			</Container>
		)
	}
}