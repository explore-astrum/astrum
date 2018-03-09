import * as React from 'react'
import * as ReactDOM from 'react-dom'
import { AppContainer } from 'react-hot-loader'
import Routes from './routes'

const element = document.getElementById('root')
const render = Component => {
	ReactDOM.render(
		<AppContainer>
			<Component />
		</AppContainer>
	, element)
}
render(Routes)
if ((module as any).hot) {
		(module as any).hot.accept('./routes', () => {
			const NextApp = require('./routes').default
			ReactDOM.render(
				<AppContainer>
					<NextApp />
				</AppContainer>
			,
			element,
		)
	})
}
