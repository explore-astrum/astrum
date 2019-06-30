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
        const loaded =
            LAYERS.filter((_item, index) => this.state[index] != true).length == 0
        return (
            <Container bg-black style={{ opacity: loaded ? 100 : 0 }}>
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
                <Container relative style={{ zIndex: 1000, background: '#212529' }}>
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
                            <Container flex flex-column align-center pad-v8 mgn-h6>
                                <Container fg-white size-4 m-size-5 weight-5 line-8 text-center>
                                    Astrum is an experiment in building a singular world owned and operated entirely by its players. Claim your piece to engage in 100% player driven trade, exploration, politics and work together to determine this future of this new world
                                </Container>
                            </Container>
                        </Container>
                        <Container pad-b8>
                            <Terrain />
                            <Container flex justify-center mgn-t4>
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
                                            Astrum is a single, persistent landscape that the players completely own. Every rock and tree is hand-crafted by our artists to create a bespoke, unique property for every land owner. After purchasing a plot of land, you and only you are able to build on top of it. No one will be able to claim any part of your land until you sell or trade it yourself.
                                    </Container>
                                    </Container>
                                </Container>
                            </Container>
                            <Container flex justify-center mgn-t8>
                                <Container flex-5 flex align-center pad-r4>
                                    <Container>
                                        <Container uppercase size-5 weight-6>Make it yours</Container>
                                        <Container mgn-t2 line-8 >
                                            Every relic found in-game and on someone’s property has been bought, traded, or sold on the marketplace. Every item is scarce, so there’s a limit to the amount of houses, vehicles, artwork, etc. You can travel to an water-side property to admire their dock and barter with the owner to improve what’s in your inventory and on your land.
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
                                            Any relics (assets) that you own can be placed on your land or combined with another relic. Make the castle or starship port of your dreams. As a creator, you can upload your own relics, set an amount and price, and release them on the marketplace.
                                    </Container>
                                    </Container>
                                </Container>
                            </Container>
                        </Container>
                        <Container fg-white>
                            <Container
                                style={{ fontFamily: 'Thruster', letterSpacing: '4px' }}
                                pad-v8
                                m-mgn-v8
                                fg-white
                                size-6
                                s-size-8
                                m-size-10
                                text-center>
                                FAQ
                            </Container>
                            <Container>
                                {
                                    FAQ.map(qa => (
                                        <Container mgn-t8 key={qa.question}>
                                            <Container size-5 weight-4 line-6 weight-6>
                                                {qa.question}
                                            </Container>
                                            <Container size-4 line-6 >
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