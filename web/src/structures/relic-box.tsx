import { NavLink } from 'react-router-dom'
import * as React from 'react'
import { component } from '../components/theme'
import * as Session from '../data/session'
import * as Relic from '../data/relic'

import { Grid, Container, Wrap } from '../components/container'
import * as Modal from '../components/modal'

interface Props {
	relic: Relic.UserRelic
}

export default function (props: Props & React.HTMLAttributes<HTMLDivElement>) {
	const { relic, ...rest } = props
	const info = Relic.info(relic.key)
	return (
		<Container {...rest} key={info.name} pad-h4 pad-v4 flex-third>
			<Container bg-white radius-4 overflow-hidden style={{
					boxShadow: '0 1px 3px rgba(0,0,0,0.12), 0 1px 2px rgba(0,0,0,0.24)'
			}}>
				<Container pad-4 style={{
					paddingBottom: '60%',
					backgroundImage: `url(${Relic.relic_image(relic.key)})`,
					backgroundPosition: 'center',
					backgroundSize: 'cover',
				}} />
				<Container size-3-5 weight-5 pad-h4 pad-v4>
					<Container flex>
						<Container flex-grow>{info.name} ({relic.balance})</Container>
						<Container fg-green>{relic.price === 0 ? 'Free' : relic.price}</Container>
					</Container>
					<Container size-3-5 weight-5 mgn-t1 fg-gray>{relic.sale || 0} For Sale</Container>
				</Container>
			</Container>
		</Container>
	)
}