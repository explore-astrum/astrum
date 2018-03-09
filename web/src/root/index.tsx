import '@ironbay/legos/dist/raw.css'
import './styles.css'
import * as React from 'react'
import Kora from '../data/kora'
import * as Session from '../data/session'
import * as User from '../data/user'

export default class Root extends React.Component<any, any> {
	componentDidMount() {
		Kora.onLocalChange.add(() => this.forceUpdate())
	}
	render() {
		const user = Session.user()
		if (user == null)
			return <span />
		return (
			<div>
			{
				React.cloneElement(this.props.children as any)
			}
			</div>
		)
	}
}
