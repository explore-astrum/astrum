import * as React from 'react'
import { Route, Switch } from 'react-router-dom'

import Kora from '../../data/kora'
import * as Session from '../../data/session'
import { Grid, Container, Wrap } from '../../components/container'
import * as Form from '../../components/form'
import Header from '../../structures/header'
import RelicPurchaseModal from '../../structures/relic-purchase-modal'
import * as Relic from '../../data/relic'
import * as Constants from '../../data/constants'
import RelicBox from '../../structures/relic-box'

const RELICS = [
	{
		name: 'Sentinel Radar',
		image: 'https://media.sketchfab.com/urls/b59212bfa2d046238cac8d570c82b9d0/dist/thumbnails/d54dc7914efb455ba5c95038265dabfe/1024x576.jpeg'
	},
	{
		name: 'Duster Ghost',
		image: 'https://media.sketchfab.com/urls/3b0ee711eb384dad9027cd6bf657ba27/dist/thumbnails/f8bbffc2bf32477683fbc6647f0a1f49/1024x576.jpeg'
	},
	{
		name: 'Slide',
		image: 'https://media.sketchfab.com/urls/bc0dab51920641fe88bae1eaccff5c4c/dist/thumbnails/4712cf60d71d416492d02b248eec2428/1024x576.jpeg'
	},
	{
		name: 'Windmill',
		image: 'https://media.sketchfab.com/urls/5b84247d5aa54bf18a84b116bd5a4b89/dist/thumbnails/b87cff587949488d930b054ca05451d1/f5ae9fb22d2a4a9ea08f17abd646e480.jpeg'
	},
	{
		name: 'Phone',
		image: 'https://media.sketchfab.com/urls/f5a08601fa1844938b95329c6f5d1a67/dist/thumbnails/f22b92426d594fb1ba67a5bd13f4ff80/ac541e56116249988a05c8a78963e405.jpeg'
	}
]

export default class ProfilePage extends React.Component<any, any> {
	constructor(props) {
		super(props)
		this.state = {
		}
	}
	componentDidMount() {
		const { user } = this.props.match.params
		Kora.query_path(['user:relics', user])
	}
	render() {
		const { user } = this.props.match.params
		const mine = Session.user() === user
		return (
			<Container>
				<Container bg-dark-gray>
					<Header dark />
					<Container pad-v8>
						<Wrap>
							<Container pad-v8 mgn-b8 fg-white>
								<Container size-8 text-center>
									{mine && 'This is your inventory'}
									{!mine && user}
								</Container>
								<Container mgn-t3 text-center pad-b6>
									{mine && 'Relics you have recently claimed may take some time to appear here'}
								</Container>
							</Container>
						</Wrap>
					</Container>
				</Container>
				<Container pad-v8 >
					<Wrap>
						<Container flex style={{
							flexWrap: 'wrap',
							width: 'calc(100% + 2rem)',
							marginLeft: '-1rem',
							marginTop: '-1rem',
							marginBottom: '-1rem',
						}} >
						{

							Relic
							.user_relics(user)
							.map(relic => (
								<RelicBox
									key={relic.key}
									relic={relic}
									onClick={() => relic.sale > 0 && this.relic_purchase_modal.show(Constants.OWNER, relic.key)} />
							))
						}
						</Container>
					</Wrap>
				</Container>
				<RelicPurchaseModal ref={r => this.relic_purchase_modal = r} />
			</Container>
		)
	}
	private relic_purchase_modal: RelicPurchaseModal
}