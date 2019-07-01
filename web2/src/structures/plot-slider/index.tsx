import * as React from 'react'
import { Container, Wrap, Image } from '../../components'
import Link from '../link'
import Lego from '../../components/lego'
import { router } from '../../data/kora'
import * as Plot from '../../data/plot'

interface Props {
    plot: string
}

export default class PlotSlider extends React.Component<Props, any> {
    componentDidMount() {

    }
    render() {
        const { plot } = this.props
        const { x, y } = Plot.key_decode(plot)
        return (
            <Container
                fg-white
                bg-black
                pad-8
                style={{
                    flexBasis: '30rem',
                    right: 0,
                    top: 0,
                    height: '100%',
                    borderTop: '4px solid #ffcc00'
                }}>
                <Container weight-6 size-6>
                    Plot {x}x{y}
                </Container>
                <Container mgn-t6 line-6 weight-5>
                    This plot named <Container inline fg-yellow weight-6>Astrum HQ</Container>, is owned by <Container inline fg-yellow weight-6>@astrum</Container>. It does not have a list price but you may make an offer.
                </Container>
                <Container mgn-t6>
                    <Container weight-6>Activity</Container>
                    <Container mgn-t4>
                        <Container>
                            <Container
                                fg-black
                                radius-4
                                weight-6
                                bg-yellow
                                pad-6 >
                                @astrum named this plot Astrum HQ
                            </Container>
                        </Container>
                    </Container>
                    <Container mgn-t4>
                        <Container>
                            <Container
                                fg-black
                                radius-4
                                weight-6
                                bg-yellow
                                pad-6 >
                                @astrum purchased this plot for $44
                            </Container>
                        </Container>
                    </Container>
                </Container>
            </Container>
        )

    }
}