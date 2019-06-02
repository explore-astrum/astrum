import * as React from 'react'
import { Container, Image, Wrap } from '../../components'

const IMG_STEP_0 = require('./img/step-0.png')
const IMG_STEP_1 = require('./img/step-1.png')
const IMG_STEP_2 = require('./img/step-2.png')


export default class LandingPage extends React.Component<any, any> {
    state = {
        scroll: 0,
        active: false
    }
    componentWillMount() {
        window.addEventListener('scroll', this.handle_scroll)
    }
    componentDidMount() { }
    render() {
        const scroll = this.state.scroll

        const LAYERS = [
            require('./img/layer-7.png'),
            require('./img/layer-6.png'),
            require('./img/layer-5.png'),
            require('./img/layer-4.png'),
            require('./img/layer-3.png'),
            require('./img/layer-2.png'),
            require('./img/layer-1.png'),
            // require('./img/layer-0.png'),
        ]
        const FRONT = require('./img/layer-0.png')
        return (
            <Container style={{ background: '#1a1e17' }}>
                {
                    LAYERS.map((item, index) => (
                        <Container key={item} fill overflow-hidden style={{
                            position: 'fixed',
                        }}>
                            <Image src={item} style={{
                                transform: `translate3d(0, ${scroll * (ease(index / LAYERS.length) * -1)}px, 0)`,
                            }} />
                        </Container>
                    ))
                }
                <Container>
                    <Image src={FRONT} relative />
                </Container>
                <Container relative style={{ background: '#1a1e17', zIndex: 1000 }}>
                    <Wrap>
                        <Container >
                            <Container
                                pad-v8
                                m-mgn-v8
                                style={{ fontFamily: 'Thruster', letterSpacing: '4px' }}
                                uppercase
                                fg-white
                                size-6
                                s-size-8
                                m-size-10
                                text-center>
                                Welcome to Astrum
                            </Container>
                            <Container>
                                <video
                                    controls
                                    style={{ width: '100%', height: 'auto' }}
                                    src={require('./img/trailer.webm')} />
                            </Container>
                            <Container flex flex-column align-center pad-v8 mgn-h6>
                                <Container fg-white size-4 m-size-5 weight-5 line-8 text-center>
                                    Astrum is an experiment in building a singular world owned and operated entirely by its players. Claim your piece to engage in 100% player driven trade, exploration, politics and work together to determine this future of this new world
                                </Container>
                                <Container mgn-t4 fg-black bg-white pad-h6 pad-v4 uppercase radius-4 weight-6 size-3-5>Browse Available Plots</Container>
                            </Container>
                        </Container>
                        <Container mgn-v8 fg-white>
                            <Container flex justify-center>
                                <Container flex-5 relative pad-r4>
                                    <Image src={IMG_STEP_2} />
                                </Container>
                                <Container flex-5 flex align-center pad-l4>
                                    <Container>
                                        <Container uppercase size-5 weight-6>Claim your piece</Container>
                                        <Container mgn-t2 line-8 >
                                            Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam fringilla ultricies risus vel posuere. Cras in urna tincidunt lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam fringilla ultricies risus vel posuere. Cras in urna tincidunt
                                    </Container>
                                    </Container>
                                </Container>
                            </Container>
                            <Container flex justify-center mgn-t8>
                                <Container flex-5 flex align-center pad-r4>
                                    <Container>
                                        <Container uppercase size-5 weight-6>Make it yours</Container>
                                        <Container mgn-t2 line-8 >
                                            Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam fringilla ultricies risus vel posuere. Cras in urna tincidunt lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam fringilla ultricies risus vel posuere. Cras in urna tincidunt
                                    </Container>
                                    </Container>
                                </Container>
                                <Container flex-5 relative pad-l4>
                                    <Image src={IMG_STEP_1} />
                                </Container>
                            </Container>
                            <Container flex justify-center mgn-t8>
                                <Container flex-5 relative pad-r4>
                                    <Image src={IMG_STEP_0} />
                                </Container>
                                <Container flex-5 flex align-center pad-l4>
                                    <Container>
                                        <Container uppercase size-5 weight-6>Explore and Trade</Container>
                                        <Container mgn-t2 line-8 >
                                            Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam fringilla ultricies risus vel posuere. Cras in urna tincidunt lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam fringilla ultricies risus vel posuere. Cras in urna tincidunt
                                    </Container>
                                    </Container>
                                </Container>
                            </Container>
                        </Container>
                    </Wrap>
                </Container>
            </Container>
        )
    }
    private handle_scroll = () => {
        this.setState({ scroll: window.scrollY })
    }
    private scroll(direction: number) {
        if (this.state.active) return
        const next = this.state.scroll + Math.sign(direction)
        if (next > 0) return
        this.setState({
            scroll: next,
            active: true,
        })
        setTimeout(() => this.setState({ active: false }), 1000)

    }

}

function ease(t) {
    return t * 0.8
}