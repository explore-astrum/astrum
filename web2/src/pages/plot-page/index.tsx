import * as React from 'react'
import { Container, Image } from '../../components'
import Header from '../../structures/header'
import Link from '../../structures/link'
import PlotSlider from '../../structures/plot-slider'
import PlotSliderEmpty from '../../structures/plot-slider-empty'
import * as Plot from '../../data/plot'
import { router, kora } from '../../data/kora'

interface Props { }
interface State {
}

const REGION_OFFSET = 64
export default class PlotPage extends React.Component<Props, State> {
    private zoom = 1
    state = {
        x: 0,
        y: 0,
    }
    ref_canvas: HTMLCanvasElement
    componentWillMount() {
        kora.query_path(['plot:info'])
    }
    render() {
        const [_, plot] = router.parts()
        return (
            <Container fill flex flex-column>
                <Header />
                <Container flex-grow flex overflow-hidden>
                    {
                        router.match_exact('/plots/+') ?
                            <PlotSlider key={plot} plot={plot} /> : <PlotSliderEmpty />
                    }
                    <Container overflow-scroll-y flex-grow>
                        <Container
                            onMouseMove={e => {
                                const rect = e.currentTarget.getBoundingClientRect()
                                const x = ((e.pageX - rect.left) / this.zoom)
                                const y = ((e.pageY - rect.top) / this.zoom)
                                const unit_x = e.currentTarget.clientWidth / 64
                                const unit_y = e.currentTarget.clientHeight / 64
                                const plot_x = Math.floor(x / unit_x)
                                const plot_y = Math.floor(y / unit_y)
                                this.setState({
                                    x: plot_x,
                                    y: plot_y,
                                })
                            }}
                            relative>
                            <Image
                                src={require('./map.png')} />
                            <Container fill>
                                <canvas ref={r => this.ref_canvas = r} style={{ width: '100%', height: '100%' }} />
                            </Container>
                            <Link href={`/plots/${Plot.key_encode(this.state.x + REGION_OFFSET, this.state.y + REGION_OFFSET)}`}>
                                <Container bg-yellow cursor-pointer style={{
                                    width: 100 / 64 + '%',
                                    height: 100 / 64 + '%',
                                    position: 'absolute',
                                    left: (this.state.x * 100 / 64) + '%',
                                    top: (this.state.y * 100 / 64) + '%',
                                }} />
                            </Link>
                        </Container>
                    </Container>
                </Container>
            </Container >
        )
    }
    componentDidMount() {
        let debounce = null
        kora.before_mutation(['plot:info', '+', 'prices'], () => {
            clearTimeout(debounce)
            debounce = setTimeout(() => this.render_canvas(), 100)
        })
    }

    render_canvas() {
        this.ref_canvas.width = 64 * 10
        this.ref_canvas.height = 64 * 10
        const ctx = this.ref_canvas.getContext('2d')
        ctx.fillStyle = 'rgba(0, 0, 0, 0.6)'
        for (let x = 0; x < 64; x++) {
            for (let y = 0; y < 64; y++) {
                const plot = Plot.key_encode(x + 64, y + 64)
                const plot_info = Plot.info(plot)
                if (!plot_info.prices.list) {
                    ctx.fillRect(x * 10, y * 10, 10, 10)
                }
            }
        }
    }
}