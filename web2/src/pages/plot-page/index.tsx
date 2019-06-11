import * as React from 'react'
import { Container, Wrap, Image } from '../../components'
import Header from '../../structures/header'
import { router } from '../../data/kora'

import Link from '../../structures/link'
import * as Plot from '../../data/plot'

import Terrain from '../../structures/terrain'
import TopDownMap from '../../structures/topdownmap'
import PlotInfo from '../../structures/plot-info'

interface Props {

}
export default class PlotPage extends React.Component<Props, State> {
    render() {
        return (
            <Container>
                <Header />
                <Container>
                    <TopDownMap />
                    <Terrain />
                </Container>
                <PlotInfo />
            </Container>
        );
    }
}