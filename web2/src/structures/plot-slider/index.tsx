import * as React from 'react'
import { Container, Image } from '../../components'
import * as Plot from '../../data/plot'
import * as User from '../../data/user'
import { kora } from '../../data/kora'

interface Props {
    plot: string
}


export default class PlotSlider extends React.Component<Props, any> {
    state = {
        image: 0
    }
    componentDidMount() {
        kora.query_path(['plot:activity', this.props.plot])
    }
    render() {
        const { plot } = this.props
        const plot_info = Plot.info(plot)
        const owner_info = User.info(plot_info.owner)
        if (!plot_info.key || !owner_info.key) {
            return (
                <Container
                    fg-white
                    bg-black
                    pad-8
                    style={{
                        flexBasis: '30rem',
                        overflowY: 'auto',
                        right: 0,
                        top: 0,
                        height: '100%',
                        borderTop: '4px solid #ffcc00'
                    }} />
            )
        }
        const { x, y } = Plot.key_decode(plot)
        const images =
            [
                'nw',
                'sw',
                'ne',
                'se',
            ].map(direction => `https://public.exploreastrum.com/iso/${plot}${direction}.jpg`)
        return (
            <Container
                fg-white
                bg-black
                pad-8
                style={{
                    flexBasis: '30rem',
                    overflowY: 'auto',
                    right: 0,
                    top: 0,
                    height: '100%',
                    borderTop: '4px solid #ffcc00'
                }}>
                <Container weight-6 size-6>
                    Plot {x}x{y}
                </Container>
                <Container mgn-t6 line-6 weight-5>
                    This plot {plot_info.name && <Container inline>named <Container inline fg-yellow weight-6>Astrum HQ</Container>,</Container>}
                    is owned by <Container inline fg-yellow weight-6>@{owner_info.username}</Container>.&nbsp;
                    {
                        !plot_info.prices.list ?
                            'It does not have a list price but you may make an offer.' :
                            <Container inline>
                                It is currently on sale for <Container inline fg-yellow>${plot_info.prices.list}</Container>
                            </Container>
                    }
                </Container>
                <Container mgn-t6 flex justify-end>
                    <Container
                        cursor-pointer
                        bg-yellow
                        fg-black
                        pad-v3
                        pad-h4
                        weight-6>
                        {
                            !plot_info.prices.list ?
                                'Make an offer →' :
                                `Purchase for $${plot_info.prices.list} →`
                        }
                    </Container>
                </Container>
                {/* <Container mgn-t6>
                    <Terrain />
                </Container> */}
                <Container mgn-t6>
                    <Container weight-6>Gallery</Container>
                    <Container
                        mgn-t4
                        cursor-pointer
                        onClick={() => this.setState({
                            image:
                                this.state.image === images.length - 1 ?
                                    0 :
                                    this.state.image + 1
                        })}>
                        <Image src={images[this.state.image]} />
                    </Container>
                    <Container flex justify-center mgn-t4>
                        <Container flex justify-between>
                            {
                                images.map((item, index) => (
                                    <Container style={{ width: '7px', height: '7px' }} radius-max bg-gray bg-white={index === this.state.image} mgn-h1 onClick={() => this.setState({ image: index })} />
                                ))
                            }

                        </Container>
                    </Container>
                </Container>
                <Container mgn-t6>
                    <Container weight-6>Activity</Container>
                    <Container>
                        {
                            Plot
                                .activity(this.props.plot)
                                .sort((a, b) => a > b ? -1 : 1)
                                .map(item => {
                                    const user_info = User.info(item.sender)
                                    return (
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
                                                <Container bg-white style={{ width: '50px', flex: '0 0 50px' }} radius-max overflow-hidden>
                                                    <Image src={`https://avatars.dicebear.com/v2/male/${item.sender}.svg`} />
                                                </Container>
                                                <Container pad-l4 line-6>
                                                    {copy(item, user_info)}
                                                </Container>
                                            </Container>
                                        </Container>
                                    )
                                })
                        }
                    </Container>
                </Container>
            </Container>
        )

    }
}

function copy(item: PlotActivity, user_info: UserInfo) {
    switch (item.type) {
        case 'plot.sold':
            return `@${user_info.username} bought this plot for $${item.data.price}`
        case 'plot.list':
            return `@${user_info.username} put this plot on sale for $${item.data.price}`
    }
}