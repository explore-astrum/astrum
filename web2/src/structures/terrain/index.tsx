import * as React from 'react'
import * as ReactDOM from 'react-dom'
import * as THREE from 'three'
import * as Chroma from 'chroma-js'
import { OrbitControls } from 'three-orbitcontrols-ts'
import { Container, Wrap } from '../../components/'
import Link from '../link'
import Lego from '../../components/lego'

import * as Plot from '../../data/plot'
import { router } from '../../data/kora'

const HEIGHTMAP = require('./heightmap.png')
const SIZE = 512
const PLOT = SIZE / (64)
const HEIGHT = 100
const WATER = 0.03
const BORDER = 1
interface Props { }
let scene = new THREE.Scene()
export default class Terrain extends React.Component<Props, any> {
    componentDidMount() {
        const root = this.root()
        const bounds = root.getBoundingClientRect()
        scene = new THREE.Scene()
        const camera = new THREE.PerspectiveCamera(45, bounds.width / bounds.height, 0.1, 10000)
        const scale = Chroma.scale(['lightgreen', 'white']).domain([0, HEIGHT * 0.8])

        const renderer = new THREE.WebGLRenderer()
        renderer.setClearColor(0x1a1e17, 1.0)
        renderer.setSize(bounds.width, bounds.height)

        const light = new THREE.DirectionalLight()
        light.position.set(SIZE / 2, 500, SIZE / 2)
        scene.add(light)

        camera.position.x = SIZE * -0.4
        camera.position.y = 500
        camera.position.z = SIZE * -0.4
        // camera.lookAt(new THREE.Vector3(SIZE / 2, 0, SIZE / 2))

        this.root().appendChild(renderer.domElement)
        const control = new OrbitControls(camera, renderer.domElement)
        control.enablePan = false
        control.enableRotate = false
        control.autoRotate = true
        control.target = new THREE.Vector3(SIZE / 2, 0, SIZE / 2)

        const geometry = new THREE.BoxGeometry(PLOT, HEIGHT, PLOT);
        const material = new THREE.MeshBasicMaterial({ color: 0xffffff, opacity: 0.5, transparent: true })
        const box = new THREE.Mesh(geometry, material);
        box.position.set(SIZE / 2, HEIGHT / 2, SIZE / 2)
        box.name = 'highlight'

        const [route, key] = router.parts()
        if(route != "landing")
            scene.add(box)


        function color(geometry, face) {
            var v1 = geometry.vertices[face.a].y;
            var v2 = geometry.vertices[face.b].y;
            var v3 = geometry.vertices[face.c].y;
            const result = Math.max(v1, v2, v3);
            if (result == HEIGHT * WATER) return 0x2364a0
            return scale(result).hex()
        }

        var depth = SIZE + BORDER * 2
        var width = SIZE + BORDER * 2
        var spacingX = 1;
        var spacingZ = 1;
        var heightOffset = 1;
        var canvas = document.createElement('canvas');
        canvas.width = depth;
        canvas.height = width;
        var ctx = canvas.getContext('2d');
        var img = new Image();
        img.src = HEIGHTMAP;
        img.onload = function () {
            // draw on canvas
            ctx.translate(width, 0);
            ctx.scale(-1, 1);
            ctx.drawImage(img, BORDER, BORDER, SIZE, SIZE);
            var pixel = ctx.getImageData(0, 0, width, depth);
            var geom = new THREE.Geometry;
            const output: number[] = []
            let max = 0;
            let min = 1000000;
            for (let i = 0; i < pixel.data.length; i += 4) {
                const r = pixel.data[i]
                const g = pixel.data[i + 1]
                const b = pixel.data[i + 2]
                const avg = (r + g + b) / 3
                output.push(avg)
                if (avg > max)
                    max = avg
                if (avg < min && avg !== 0)
                    min = avg
            }
            let i = 0
            for (var x = 0; x < depth; x++) {
                for (var z = 0; z < width; z++) {
                    // get pixel
                    // since we're grayscale, we only need one element
                    var yValue = Math.max(WATER, (output[i] - min) / (max - min))
                    var vertex = new THREE.Vector3(x * spacingX, yValue * HEIGHT, z * spacingZ);
                    geom.vertices.push(vertex);
                    i++
                }
            }
            // we create a rectangle between four vertices, and we do
            // that as two triangles.
            for (var z = 0; z < depth - 1; z++) {
                for (var x = 0; x < width - 1; x++) {
                    // we need to point to the position in the array
                    // a - - b
                    // |  x  |
                    // c - - d
                    var a = x + z * width;
                    var b = (x + 1) + (z * width);
                    var c = x + ((z + 1) * width);
                    var d = (x + 1) + ((z + 1) * width);
                    var face1 = new THREE.Face3(a, b, d);
                    var face2 = new THREE.Face3(d, c, a);
                    face1.color = new THREE.Color(color(geom, face1));
                    face2.color = new THREE.Color(color(geom, face2));
                    geom.faces.push(face1);
                    geom.faces.push(face2);
                }
            }
            geom.computeVertexNormals(true);
            geom.computeFaceNormals();
            geom.computeBoundingBox();
            var zMax = geom.boundingBox.max.z;
            var xMax = geom.boundingBox.max.x;
            var mesh = new THREE.Mesh(geom, new THREE.MeshLambertMaterial({
                vertexColors: THREE.FaceColors,
                color: 0x666666,
                shading: THREE.SmoothShading
            }));
            mesh.material.side = THREE.DoubleSide;
            mesh.translateX(-BORDER);
            mesh.translateZ(-BORDER);
            scene.add(mesh);
            mesh.name = 'valley';
        };

        const render = () => {
            control.update()
            renderer.render(scene, camera)
            requestAnimationFrame(render)
        }
        render()
    }
    private root() {
        return ReactDOM.findDOMNode(this) as HTMLElement
    }

    render() {
        const [route, key] = router.parts()
        if(route != "landing" && key) {
            const { x, y } = Plot.key_decode(key)
            let box = scene.children.filter(i => i.name === 'highlight')[0]
            if(box)
                box.position.set(y * PLOT - SIZE, HEIGHT / 2, (64 + 128 - x) * PLOT - SIZE);
        }
        return <Container style={{ height: '50rem' }} />
    }
}
