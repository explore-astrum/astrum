import '@ironbay/legos/dist/raw.css'
import './styles.css'
import * as React from 'react'
import { Container } from '../../components'
import { kora, router } from '../../data/kora'

import LandingPage from '../landing-page'
import PlotPage from '../plot-page'

interface Props {

}

interface State {

}
export default class RootPage extends React.Component<any, any> {
    render() {
        return (
            <Container>
                {router.match_prefix('/landing') && <LandingPage />}
                {router.match_prefix('/plot/+') && <PlotPage />}
            </Container>
        )
    }
}