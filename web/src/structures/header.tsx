import { NavLink } from 'react-router-dom'
import * as React from 'react'
import { component } from '../components/theme'
import * as Session from '../data/session'

import { Grid, Container, Wrap } from '../components/container'

export default function Header(props) {
	const user = Session.user()
	return (
		<Container pad-v8>
			<Wrap>
				<Container flex>
					<NavLink to="/">
						<Container uppercase size-5 weight-6 fg-white={props.dark} fg-black={!props.dark} >astrum</Container>
					</NavLink>
					<Container flex-grow />
					{!user && <Link {...props} to="/auth/email">Enter</Link>}
					{user && <Link {...props} to="/land">Claim Land</Link>}
					{user && <Link {...props} to="/marketplace">Marketplace</Link>}
					{user && <Link {...props} to={`/profile/${user}`}>Profile</Link>}
				</Container>
			</Wrap>
		</Container>
	)
}

const Link = props => {
	return (
		<NavLink to={props.to} className={`uppercase size-3 ${props.dark ? 'fg-white' : 'fg-gray'} cursor-pointer weight-6 mgn-l8`} activeClassName="fg-green">
			{props.children}
		</NavLink>
	)
}