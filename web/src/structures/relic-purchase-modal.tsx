import { NavLink } from 'react-router-dom'
import * as React from 'react'
import { component } from '../components/theme'
import * as Session from '../data/session'
import * as Relic from '../data/relic'
import * as Neo from '../data/neo'

import { Grid, Container, Wrap } from '../components/container'
import { Image } from '../components/image'
import * as Modal from '../components/modal'

interface Props {

}

interface State {
	relic?: string
	owner?: string
	pending: boolean
	active: boolean
}

export default class RelicPurchaseModal extends React.Component<Props, State> {
	constructor(props) {
		super(props)
		this.state = {
			active: false,
			pending: false,
		}
	}

	public show(owner, relic) {
		this.setState({
			owner,
			relic,
			active: true,
		})
	}

	public hide() {
		this.setState({
			active: false,
			pending: false,
		})
	}

	public render() {
		const { active, pending } = this.state
		return (
			<Modal.Root onHide={() => this.hide()} active={active}>
				<Modal.Title>Claim This Relic</Modal.Title>
				<Container>
					<Image src={Relic.relic_image(this.state.relic)} />
				</Container>
				{!pending && <Modal.Footer onClick={() => this.handle_claim()} >Claim</Modal.Footer>}
				{pending && <Modal.Footer>Submitting...</Modal.Footer>}
			</Modal.Root>
		)
	}

	private handle_claim() {
		this.setState({
			pending: true
		})
		const { relic, owner } = this.state
		Neo.relic_buy(owner, relic, 1)
		.then(() => {
			alert('Your transaction has been submitted to the blockchain.  It should be confirmed within a minute')
			this.hide()
		})
		.catch(() => {
			alert('For the time being we only support one pending transaction at a time.  Your wallet is not ready for another transaction yet, please wait a few moments')
			this.hide()
		})
		
	}

}