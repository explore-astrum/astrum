import '@ironbay/legos/dist/raw.css'
import './styles.css'
import * as React from 'react'
import { Container } from '../../components'
import { kora, router } from '../../data/kora'
import * as Session from '../../data/session'

import LandingPage from '../landing-page'
import PlotPage from '../plot-page'
import AuthPage from '../auth-page'

interface Props {

}

interface State {

}
export default class RootPage extends React.Component<any, any> {
    render() {
        // if (Session.user() == null) return false
        return (
            <Container>
                {router.match_prefix('/landing') && <LandingPage />}
                {router.match_prefix('/auth') && <AuthPage />}
                {router.match_prefix('/plots') && <PlotPage />}
            </Container>
        )
    }
}