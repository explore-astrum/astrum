import * as React from 'react'
import { Container, Image, Wrap } from '../../components'
import Terrain from '../../structures/terrain'
import { formatWithOptions } from 'util';

const IMG_STEP_0 = require('./img/step-0.png')
const IMG_STEP_1 = require('./img/step-1.png')
const IMG_STEP_2 = require('./img/step-2.png')

const IMG_VIDEO = require('./img/video.png')

const FAQ = [
    { question: "How do I buy or sell on the marketplace?", answer: "The marketplace can be found here." },
    { question: "How much does the game cost?", answer: "The game is free to play. But to participate in the marketplace and manage an inventory, you must purchase a plot of land here." },
    { question: "What happens when all of the land is sold", answer: "We will be releasing new, continuous landscape to grow Astrum in the future." },
    { question: "Can I participate if I don't have a way to play the game?", answer: "Yes! You can still buy, trade, sell, manage your inventory, and cash out without ever opening the game." },
    { question: "How do I upload my own assets / relics?", answer: "We will add this shortly. Stay tuned." },
    { question: "When will the game be released?", answer: "This fall." },
    { question: "What platforms will you support?", answer: "PC at first." },
    { question: "Can I cash out to a real-world currency?", answer: "Yes! At any point, you can trade your dust (in-game currency) for cash." },
    { question: "What do I start with?", answer: "TBD" },
    { question: "Can I grow my inventory without spending more money?", answer: "Yes! You can always trade to get what you want. But also, there will be a drop of random relics every month to each player, to provide players with a new fresh set of goods." },
    { question: "As a creator, what file types do you accept?", answer: "We will accept .fbx files. The asset itself will be vetted by a team of curators to make sure that it doesn’t resemble any current relics and is a viable asset for the game." },
    { question: "How do I combine my relics?", answer: "Only certain relics can be combined. Some are purely aesthetic combinations (paint on your house). And others are necessary (oil in your vehicle to drive it, or water on your garden to make it grow)." },
    { question: "What does persistent mean?", answer: "This means that every player in Astrum will witness any additions or changes to your land. If you place a house, then any other landowner can come to your property and take notice, even when you're logged off." }
]


export default class LandingPage extends React.Component<any, any> {
    state = {
        scroll: 0,
        active: false,
        playing: false
    }
    refs_video: HTMLVideoElement
    componentWillMount() {
        window.addEventListener('scroll', this.handle_scroll)
    }
    componentDidMount() {
    }
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
        // const loaded =
        // LAYERS.filter((_item, index) => this.state[index] != true).length == 0
        const loaded = true
        return (
            <Container fg-white bg-black style={{ opacity: loaded ? 100 : 0 }}>
                {
                    LAYERS.map((item, index) => (
                        <Container key={item} fill overflow-hidden style={{
                            position: 'fixed',
                        }}>
                            <Image
                                onLoad={() => this.setState({ [index]: true })}
                                src={item}
                                style={{
                                    transform: `translate3d(0, ${scroll * (ease(index / LAYERS.length) * -1)}px, 0)`,
                                }} />
                        </Container>
                    ))
                }
                <Container>
                    <Image src={FRONT} relative />
                </Container>
                <Container relative style={{ zIndex: 1000, background: '#000000' }}>
                    <Wrap>
                        <Container >
                            <Container
                                pad-v8
                                m-mgn-v8
                                fg-white>
                                <Container text-center size-6>
                                    A new world to conquer
                                </Container>
                                <Container text-center size-10 weight-8 mgn-t4>
                                    Welcome to Astrum
                                </Container>
                            </Container>
                            <Container relative cursor-pointer>
                                <video
                                    ref={r => this.refs_video = r}
                                    onClick={() => this.refs_video.paused ? this.refs_video.play() : this.refs_video.pause()}
                                    controls={false}
                                    style={{ width: '100%', height: 'auto' }}
                                    src="https://public.exploreastrum.com/trailer.webm" />
                                {
                                    !this.state.playing &&
                                    <Container fill onClick={() => {
                                        this.refs_video.play()
                                        this.setState({
                                            playing: true
                                        })
                                    }}>
                                        <Image src={IMG_VIDEO} />
                                    </Container>
                                }
                            </Container>
                            <Container
                                mgn-t8
                                flex
                                fg-white
                                size-5
                                weight-5
                                line-8
                                text-center>
                                <Container flex-5 pad-r4>
                                    Astrum is an experiment in building a singular world owned and operated entirely by you and your fellow inhabitants
                                </Container>
                                <Container flex-5 pad-l4>
                                    Claim a piece to engage in 100% player driven trade, exploration, politics and work together to determine this future of this new world
                                </Container>
                            </Container>
                        </Container>
                        <Container flex justify-center pad-t8>
                            <Container
                                fg-black
                                bg-yellow
                                pad-h6
                                pad-v6
                                cursor-pointer
                                uppercase
                                weight-8
                            >Browse Available Plots →</Container>
                        </Container>
                        <Container fg-white mgn-t8>
                            <Container flex align-center pad-t4>
                                <Container flex-5 pad-r4>
                                    <Container flex align-center>
                                        <Container
                                            style={{ fontSize: '6rem' }}
                                            weight-6>01</Container>
                                        {/* <Container size-5 pad-l4 weight-5>Claim your piece</Container> */}
                                    </Container>
                                    <Container weight-6 mgn-t8 size-5 >
                                        Claim your piece
                                    </Container>
                                    <Container line-8 mgn-t8>
                                        <Container>
                                            Astrum is a single, persistent landscape that the players completely own. Every rock and tree is hand-crafted by our artists to create a bespoke, unique property for every land owner.
                                        </Container>
                                        <Container mgn-t8>
                                            After purchasing a plot of land, you and only you are able to build on top of it. No one will be able to claim any part of your land until you sell or trade it yourself.
                                        </Container>
                                    </Container>
                                </Container>
                                <Container flex-5 pad-l4>
                                    <Image src={IMG_STEP_2} />
                                </Container>
                            </Container>
                            <Container flex align-center mgn-t8 pad-t8>
                                <Container flex-5 pad-r4>
                                    <Image src={IMG_STEP_1} />
                                </Container>
                                <Container flex-5 pad-l4>
                                    <Container flex align-center>
                                        <Container
                                            style={{ fontSize: '6rem' }}
                                            weight-6>02</Container>
                                        {/* <Container size-5 pad-l4 weight-5>Claim your piece</Container> */}
                                    </Container>
                                    <Container weight-6 mgn-t8 size-5>
                                        Make it your own
                                    </Container>
                                    <Container line-8 mgn-t8>
                                        <Container>
                                            Astrum is a single, persistent landscape that the players completely own. Every rock and tree is hand-crafted by our artists to create a bespoke, unique property for every land owner.
                                        </Container>
                                        <Container mgn-t8>
                                            After purchasing a plot of land, you and only you are able to build on top of it. No one will be able to claim any part of your land until you sell or trade it yourself.
                                        </Container>
                                    </Container>
                                </Container>
                            </Container>
                            <Container flex align-center pad-t8 mgn-t8>
                                <Container flex-5 pad-r4>
                                    <Container flex align-center>
                                        <Container
                                            style={{ fontSize: '6rem' }}
                                            weight-6>03</Container>
                                        {/* <Container size-5 pad-l4 weight-5>Claim your piece</Container> */}
                                    </Container>
                                    <Container weight-6 mgn-t8 size-5>
                                        Explore and trade
                                    </Container>
                                    <Container line-8 mgn-t8>
                                        <Container>
                                            Astrum is a single, persistent landscape that the players completely own. Every rock and tree is hand-crafted by our artists to create a bespoke, unique property for every land owner.
                                        </Container>
                                        <Container mgn-t8>
                                            After purchasing a plot of land, you and only you are able to build on top of it. No one will be able to claim any part of your land until you sell or trade it yourself.
                                        </Container>
                                    </Container>
                                </Container>
                                <Container flex-5 pad-l4>
                                    <Image src={IMG_STEP_0} />
                                </Container>
                            </Container>
                        </Container>
                        <Container mgn-t8 pad-t4>
                            <Container text-center size-10 weight-8>
                                FAQ
                            </Container>
                            <Container>
                                {
                                    FAQ.map(qa => (
                                        <Container mgn-t8 line-8 key={qa.question} style={{ borderLeft: '4px solid #fc0' }} pad-l4>
                                            <Container size-4 weight-4 line-6 weight-6>
                                                {qa.question}
                                            </Container>
                                            <Container size-4 >
                                                {qa.answer}
                                            </Container>
                                        </Container>
                                    ))
                                }
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