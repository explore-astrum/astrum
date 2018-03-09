import * as React from 'react'
import { Route, BrowserRouter, Switch, Redirect } from 'react-router-dom'

import Root from './root'
import HomePage from './pages/home-page'
import LandPage from './pages/land-page'
import AuthEmail from './pages/auth-page/email'
import AuthVerify from './pages/auth-page/verify'
import MarketplacePage from './pages/marketplace-page'
import ProfilePage from './pages/profile-page'

export default () => {
	return (
		<BrowserRouter>
			<Root>
				<Switch>
					<Route path="/" exact component={HomePage} />
					<Route path="/marketplace" component={MarketplacePage} />
					<Route path="/land" component={LandPage} />
					<Route component={ProfilePage} exact path="/profile/:user" />
					<Route component={AuthEmail} exact path="/auth/email" />
					<Route component={AuthVerify} exact path="/auth/verify/:token?" />
				</Switch>
			</Root>
		</BrowserRouter>
	)
}
