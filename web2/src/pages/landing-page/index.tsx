import * as React from 'react'
import * as THREE from 'three'
import * as Chroma from 'chroma-js'

const IMG_HILL = require('./img/hill.png')
const IMG_MOUNTAINS = require('./img/mountains.png')
const IMG_SKY = require('./img/sky.png')
const IMG_TREES = require('./img/trees.png')


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
                    <div style={{
                        position: 'absolute',
                        top: 0,
                        left: 0,
                        right: 0,
                        bottom: 0,
                        backgroundImage: `url(${IMG_SKY})`,
                        backgroundSize: '100% auto',
                        backgroundPosition: 'center 0',
                        backgroundRepeat: 'repeat-x',
                        transform: `translate3d(0, ${scroll * -0.25}px, 0)`
                    }} />
                    <div style={{
                        position: 'absolute',
                        top: 0,
                        left: 0,
                        right: 0,
                        bottom: 0,
                        backgroundImage: `url(${IMG_MOUNTAINS})`,
                        backgroundSize: '100% auto',
                        backgroundPosition: 'center 0',
                        backgroundRepeat: 'repeat-x',
                        transform: `translate3d(0, ${scroll * -0.25}px, 0)`
                    }} />
                    <div style={{
                        position: 'absolute',
                        top: 0,
                        left: 0,
                        right: 0,
                        bottom: 0,
                        backgroundImage: `url(${IMG_TREES})`,
                        backgroundSize: '100% auto',
                        backgroundPosition: 'center 0',
                        backgroundRepeat: 'repeat-x',
                        transform: `translate3d(0, ${scroll * -0.5}px, 0)`
                    }} />
                    <div style={{
                        position: 'absolute',
                        top: 0,
                        left: 0,
                        right: 0,
                        bottom: 0,
                        backgroundImage: `url(${IMG_HILL})`,
                        backgroundSize: '100% auto',
                        backgroundPosition: 'center 0',
                        backgroundRepeat: 'repeat-x',
                        transform: `translate3d(0, ${scroll * -1}px, 0)`
                    }} />
                </div>
                <div style={{ width: '100%', paddingBottom: '56.2%' }} />
                <div style={{ position: 'relative', height: '3000px', background: 'black', }} >
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