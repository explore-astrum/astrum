import * as React from 'react'
import { Container, Wrap, Image } from '../../components'
import Header from '../../structures/header'
import { router } from '../../data/kora'
import * as Plot from '../../data/plot'

interface Props {

}

interface State {

}
const IMG_SAMPLE = require('./sample.png')
export default class PlotPage extends React.Component<any, any> {
    componentWillMount() {
        const [_, plot] = router.parts()
        Plot.refresh(plot)
    }
    render() {
        const [_, key] = router.parts()
        const plot = Plot.info(key)
        // if (!plot.key) return false
        const coords = Plot.key_decode(key)
        return (
            <Container>
                <Header />
                <Container flex pad-h8>
                    <Container flex flex-grow bg-light-gray pad-4 relative>
                        <Container bg-white fill style={{ backgroundImage: `url(${IMG_SAMPLE})`, backgroundSize: 'cover' }} />
                    </Container>
                    <Container flex-third bg-light-gray>
                        <Container style={{ width: '100%', paddingBottom: '100%' }} relative>
                            <Container fill>
                                {/* <iframe src="http://localhost:36213/" frameBorder="0" width="100%" height="100%" style={{ background: 'transparent', margin: 0, padding: 0, }} /> */}
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
                        <Container flex-5>
                            <Container weight-5 size-5 mgn-t8 >Neighboring Plots</Container>
                            <Container mgn-t4>
                                <Container flex>
                                    <Image mgn-r2 src={IMG_SAMPLE} />
                                    <Image mgn-l2 src={IMG_SAMPLE} />
                                </Container>
                                <Container flex mgn-t4>
                                    <Image mgn-r2 src={IMG_SAMPLE} />
                                    <Image mgn-l2 src={IMG_SAMPLE} />
                                </Container>
                            </Container>
                        </Container>
                    </Container>
                </Container>
            </Container>
        )
    }
}