import * as React from 'react'
import { Container, Image } from '../../components'
import * as Plot from '../../data/plot'
import Terrain from '../terrain'

interface Props {
}


export default class PlotSliderEmpty extends React.Component<Props, any> {
    state = {
    }
    componentDidMount() {

    }
    render() {
        return (
            <Container
                fg-white
                bg-black
                flex
                pad-8
                style={{
                    flexBasis: '30rem',
                    overflowY: 'auto',
                    right: 0,
                    top: 0,
                    height: '100%',
                    borderTop: '4px solid #ffcc00'
                }}>
                <Container>
                    <Container weight-6 size-6>
                        Plot Explorer
                    </Container>
                    <Container mgn-t6 line-6 weight-5>
                        This is the Astrum Plot Explorer. It can be used to purchase a plot, view ownership history and browse photos.
                        <br />
                        <br />
                        To get started select a plot from the map to the right
                    </Container>

                    <Container mgn-t8>
                        <Container weight-6>Recent Purchases</Container>
                        <Container>
                            <Container mgn-t4>
                                {/* <Container flex justify-end>
                            <Container pad-v2 size-3-5 weight-6>5:20pm Feb 23</Container>
                        </Container> */}
                                <Container
                                    flex
                                    fg-black
                                    weight-6
                                    bg-yellow
                                    align-center
                                    pad-4 >
                                    <Container style={{ width: '50px', flex: '0 0 50px' }} radius-max overflow-hidden>
                                        <Image src="http://identicon.net/img/identicon.png" />
                                    </Container>
                                    <Container pad-l4 line-6>@astrum purchased plot 64x64 for $50</Container>
                                </Container>
                            </Container>
                            <Container mgn-t4>
                                {/* <Container flex justify-end>
                            <Container pad-v2 size-3-5 weight-6>5:20pm Feb 23</Container>
                        </Container> */}
                                <Container
                                    flex
                                    fg-black
                                    weight-6
                                    bg-yellow
                                    align-center
                                    pad-4 >
                                    <Container style={{ width: '50px', flex: '0 0 50px' }} radius-max overflow-hidden>
                                        <Image src="http://identicon.net/img/identicon.png" />
                                    </Container>
                                    <Container pad-l4 line-6>@astrum purchased plot 78x89 for $100</Container>
                                </Container>
                            </Container>
                        </Container>
                    </Container>
                </Container>
            </Container>
        )

    }
}