import * as React from 'react'
import * as THREE from 'three'
import * as Chroma from 'chroma-js'

const IMG_HILL = require('./img/hill.png')
const IMG_MOUNTAINS = require('./img/mountains.png')
const IMG_SKY = require('./img/sky.png')
const IMG_TREES = require('./img/trees.png')

require('./img/layer-0.png')
require('./img/layer-1.png')
require('./img/layer-2.png')
require('./img/layer-3.png')
require('./img/layer-4.png')
require('./img/layer-5.png')
require('./img/layer-6.png')
require('./img/layer-7.png')


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
        return (
            <div>
                <div style={{ position: 'fixed', width: '100%', paddingBottom: '56.2%', overflow: 'hidden' }}>
                    {
                        [
                            require('./img/layer-0.png'),
                            require('./img/layer-1.png'),
                            require('./img/layer-2.png'),
                            require('./img/layer-3.png'),
                            require('./img/layer-4.png'),
                            require('./img/layer-5.png'),
                            require('./img/layer-6.png'),
                            require('./img/layer-7.png')
                            // IMG_SKY,
                            // IMG_MOUNTAINS,
                            // IMG_TREES,
                            // IMG_HILL,
                        ]
                            .map((layer, index) => (
                                <div style={{
                                    position: 'absolute',
                                    top: 0,
                                    left: 0,
                                    right: 0,
                                    bottom: 0,
                                    backgroundImage: `url(${layer})`,
                                    backgroundSize: '100% auto',
                                    backgroundPosition: 'center 0',
                                    backgroundRepeat: 'repeat-x',
                                    transform: `translate3d(0, ${scroll * -((index / 7))}px, 0)`,
                                }} />

                            ))
                    }
                </div>
                <div style={{ width: '100%', paddingBottom: '33.47%' }} />
                <div style={{ position: 'relative', height: '3000px', background: '#282328', }} >
                    <canvas id="canvas" style={{ width: '100%', height: '500px' }} />
                </div>
            </div>
        )
    }
    private handle_keydown = (e: KeyboardEvent) => {
        if (e.which === 38) this.scroll(1)
        if (e.which === 40) this.scroll(-1)
    }
    private handle_scroll = (e: WheelEvent) => {
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