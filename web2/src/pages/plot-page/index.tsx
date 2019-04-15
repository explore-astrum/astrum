import * as React from 'react'
import { Container, Wrap, Image } from '../../components'
import Header from '../../structures/header'
import { router } from '../../data/kora'

import Link from '../../structures/link'
import * as Plot from '../../data/plot'

interface Props {

}

interface State {
    direction: string
}
const IMG_SAMPLE = require('./sample.png')
export default class PlotPage extends React.Component<Props, State> {
    state = {
        direction: 'nw'
    }
    componentWillMount() {
        const [_, plot] = router.parts()
        Plot.refresh(plot)
    }
    render() {
        const [_, key] = router.parts()
        const { x, y } = Plot.key_decode(key)
        const plot = Plot.info(key)
        // if (!plot.key) return false
        const coords = Plot.key_decode(key)
        const { direction } = this.state
        return (
            <Container>
                <Header />
                <Container flex>
                    <Container flex-2>
                        <Image src={`https://public.exploreastrum.com/iso/${key}nw.jpg`} />
                        <Image src={`https://public.exploreastrum.com/iso/${key}sw.jpg`} />
                        <Image src={`https://public.exploreastrum.com/iso/${key}ne.jpg`} />
                        <Image src={`https://public.exploreastrum.com/iso/${key}se.jpg`} />
                    </Container>
                    <Container flex-grow style={{
                        backgroundImage: `url(https://public.exploreastrum.com/iso/${key}${direction}.jpg)`,
                        backgroundSize: 'cover',
                        backgroundPosition: 'center',
                    }}>
                        <Container flex justify-end>
                            <Container pad-8 flex fg-white align-center justify-center bg-black size-6 weight-6 uppercase>
                                <Container>
                                    <Container text-center>Plot</Container>
                                    <Container mgn-t2>{x} x {y}</Container>
                                </Container>
                            </Container>
                        </Container>
                    </Container>
                </Container>
                <Container pad-h8>
                    <Container pad-t8 flex>
                        <Container flex-grow>
                            <Container weight-5 size-6>Plot {coords.x}x{coords.y} {plot.name && '- ' + plot.name}</Container>
                            <Container mgn-t3 size-4>Property of <Container fg-blue inline weight-6>@{plot.owner || 'astrum'}</Container></Container>
                        </Container>
                        <Container>
                            <Container
                                bg-pink
                                weight-4
                                size-4
                                weight-5
                                pad-v4
                                radius-4
                                pad-h8
                                style={{ color: 'rgba(0, 0, 0, 0.6)' }} > Make an Offer</Container>
                        </Container>
                    </Container>
                    <Container flex>
                        <Container flex-5>
                            <Container weight-5 size-5 mgn-t8 >History</Container>
                            <Container>
                                <Container style={{ borderLeft: '2px solid lightgray' }} pad-l4 mgn-t6>
                                    <Container weight-5>Sold</Container>
                                    <Container mgn-t2>
                                        <Container inline weight-6 >@astrum</Container> sold plot to <Container inline weight-6>@thdxr</Container> for <Container inline weight-5>$20.00</Container>
                                    </Container>
                                </Container>
                                <Container style={{ borderLeft: '2px solid lightgray' }} pad-l4 mgn-t6>
                                    <Container weight-5>Offer</Container>
                                    <Container mgn-t2>
                                        <Container inline weight-6>@thdxr</Container> offered <Container inline weight-6>@astrum</Container> <Container inline weight-5>$20.00</Container>
                                    </Container>
                                </Container>
                                <Container style={{ borderLeft: '2px solid lightgray' }} pad-l4 mgn-t6>
                                    <Container weight-5>Listing</Container>
                                    <Container mgn-t2>
                                        <Container inline weight-6 >@astrum</Container> listed plot for <Container inline weight-5>$30.00</Container>
                                    </Container>
                                </Container>
                            </Container>
                        </Container>
                    </Container>

                    <Container flex-5>
                        <Container weight-5 size-5 mgn-t8 >Neighboring Plots</Container>
                        <Container mgn-t4 flex>
                            {
                                [
                                    [1, 0],
                                    [0, 1],
                                    [0, -1],
                                    [-1, 0],
                                ]
                                    .map(([dx, dy]) => {
                                        const key = Plot.key_encode(x + dx, y + dy)
                                        const direction = ['sw', 'nw', 'se', 'sw'][Math.floor(Math.random() * 4)]
                                        return (
                                            <Container flex pad-h1 flex-2>
                                                <Link href={`/plot/${key}`}>
                                                    <Image src={`https://public.exploreastrum.com/iso/${key}${direction}.jpg`} />
                                                </Link>
                                            </Container>


                                        )
                                    })
                            }
                        </Container>
                    </Container>
                </Container>
            </Container>
        )
    }
}