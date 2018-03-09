import './styles.css'
import * as React from 'react'
import { default as Neon, wallet } from '@cityofzion/neon-js'

import { Grid, Container, Wrap } from '../../components/container'
import { Image } from '../../components/image'
import Header from '../../structures/header'
import * as Form from '../../components/form'

import * as Constants from '../../data/constants'
import Kora from '../../data/kora'
import * as Neo from '../../data/neo'
import * as Session from '../../data/session'

const LANDSCAPE_IMG = require('./landscape.png')
const DESERT_IMG = require('./desert.png')
const CLEFT_IMG = require('./cleft.svg')
const WORLD_IMG = require('./world.svg')
const MESH_IMG = require('./mesh.png')
const RECORD = require('./record.png')
const RECORD_WIRE = require('./record_wire.png')
const SHEPHERD = require('./shepherd.png')
const SUBWAY = require('./subway.png')
const RELIC_LANDSCAPE = require('./city.png')
const SHEPHERD_LOCK = require('./shepherd_lock.png')

interface State {
}

export default class HomePage extends React.Component<any, State> {
	constructor(props, context) {
		super(props, context)
		this.state = {}
	}
	async componentDidMount() {
		// Neo.transfer_plot(20, 45, Session.user()).then(console.dir)
	}
	render() {
		return (
			<Container>
				<Header />
				<Container>
					<Wrap>
						<Container m-flex align-center mgn-v8>
							<Container flex-grow pad-r6 fg-dark-gray>
								<Container weight-6 size-12 line-5 text-center m-text-left>Discover<br/>Claim<br/>Curate</Container>
								<Container fg-gray size-6 line-6 mgn-t2 text-center m-text-left>A world powered by blockchain and owned by its community</Container>
							</Container>
							<Container flex-6>
								<Image src={DESERT_IMG} />
							</Container>
						</Container>
					</Wrap>
				</Container>
				<Container pad-v8 style={{
					backgroundImage: `url(${CLEFT_IMG})`,
					backgroundSize: 'cover',
					backgroundPosition: 'center 0',
				}}>
					<Wrap>
						<Container flex style={{padding: '15rem 0 5rem 0'}} align-center>
							<Container flex-5 hide m-block mgn-r8 pad-r8>
								<Image src={RELIC_LANDSCAPE} />
							</Container>
							<Container flex-10 m-flex-5>
								<Container fg-green size-5 uppercase text-center m-text-left>Welcome to Astrum</Container>
								<Container size-8 line-5 text-center m-text-left fg-white mgn-t2>Humanity's next home</Container>
								<Container fg-white-50 line-7 mgn-t2 text-center m-text-left>Astrum is a beautiful, hand-crafted world full of detail.  Featuring multiple biomes with immersive foliage and terrain, an endless landscape provides unique pockets to explore with your favorite mode of transportation.</Container>
							</Container>
						</Container>
					</Wrap>
				</Container>
				<Container pad-v8 bg-light-gray>
					<Wrap>
						<Container pad-v8>
							<Container size-8 uppercase text-center pad-v8>Own Your Reality</Container>
							<Container mgn-t6 block m-flex>
								<Card title="Make it yours" icon={RECORD}>
									Claim a beautiful acre and improve it by collecting, curating, and interacting with relics. Build your dream lair or castle to coexist with your environment. You’ll keep your plot, your neighbors, and your community.  Explore Astrum to discover what others have collected and watch the world evolve.
								</Card>
								<Card title="Community Driven" icon={SUBWAY}>
								Astrum is owned by the players.  Band together with other players to form your own districts, elect your own leaders, distribute group relics, and declare your own rules.  Astrum’s destiny is up to you.
								</Card>
								<Card title="100% Open Source" icon={RECORD_WIRE}>
								We’re open to all kinds of contributors.  Developers can help improve the core platform and artists can create and price relics that will be published on the open marketplace. Whether you’re a collector or a creator, find your home in Astrum.
								</Card>
							</Container>
						</Container>
					</Wrap>
				</Container>
				<Container pad-v8 bg-white>
					<Wrap>
						<Container flex pad-v8 align-center>
							<Container flex-10 m-flex-5 pad-v8>
								<Container size-5 uppercase text-center m-text-left>True Scarcity</Container>
								<Container size-8 line-5 text-center m-text-left mgn-t2>Enforced by blockchain</Container>
								<Container fg-gray line-7 mgn-t2 text-center m-text-left>
								The land you own is one of a kind, and every single relic in Astrum lives on the blockchain, which means they are 100% owned by you. They cannot be replicated, altered, or taken away from you and there are only a few copies of every relic.  And unlike other pay-to-win models, you will always have a means of collecting even the rarest relics.
								</Container>
							</Container>
							<Container flex-5 pad-l8 mgn-l6 hide m-block>
								<Image src={SHEPHERD_LOCK} />
							</Container>
						</Container> </Wrap>
				</Container>
			</Container>
		)
	}
}

function Card(props: {icon: string, title: string, children: any}) {
	return (
		<Container flex-grow pad-2>
			<Container bg-white pad-6 radius-2 style={{
				boxShadow: '0 1px 3px rgba(0,0,0,0.12), 0 1px 2px rgba(0,0,0,0.24)',
				height: '27rem'
			}}>
				<Container text-center>
				<Container inline-block style={{width: '100px', height: '110px'}}>
					<Image src={props.icon}  />
					</Container>
				</Container>
				<Container line-6 size-3-5 uppercase weight-5 mgn-t6>{props.title}</Container>
				<Container mgn-t1 size-3-5 line-7 fg-gray>
					{props.children}
				</Container>
			</Container>
		</Container>
	)
}