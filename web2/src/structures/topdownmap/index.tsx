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
const DIST_FROM_CAMERA = -700
const renderer = new THREE.WebGLRenderer()
const scene = new THREE.Scene()
interface Props { }
export default class TopDownMap extends React.Component<Props, any> {
    componentDidMount() {
        const root = this.root()
        const bounds = root.getBoundingClientRect()
        const camera = new THREE.OrthographicCamera(- bounds.width / 2, bounds.width / 2, bounds.height / 2, - bounds.height / 2, 1, 1000)
        const scale = Chroma.scale(['lightgreen', 'white']).domain([0, HEIGHT * 0.8])

        renderer.setClearColor(0x1a1e17, 1.0)
        renderer.setSize(bounds.width, bounds.height)

        const light = new THREE.DirectionalLight()
        light.position.set(SIZE / 2, 500, SIZE / 2)
        scene.add(light)

        this.root().appendChild(renderer.domElement)

        const geometry = new THREE.BoxGeometry(PLOT, PLOT, PLOT);
        const material = new THREE.MeshBasicMaterial({ color: 0xff0000, opacity: 1.0, transparent: true })
        const box = new THREE.Mesh(geometry, material);
        box.position.set(0, 0,DIST_FROM_CAMERA)


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

            let i = 0
            for (var x = 0; x < depth; x++) {
                for (var z = 0; z < width; z++) {
                    // get pixel
                    // since we're grayscale, we only need one element
                    var yValue = 0;
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
                    const index = (z*width + x) * 4;
                    const red = pixel.data[index] / 255.0 * 3
                    const green = pixel.data[index + 1] / 255.0 * 3
                    const blue = pixel.data[index + 2] / 255.0 * 3
                    face1.color = new THREE.Color(red, green, blue);
                    face2.color = new THREE.Color(red, green, blue);
                    geom.faces.push(face1);
                    geom.faces.push(face2);
                }
            }
            geom.computeVertexNormals(true);
            geom.computeFaceNormals();
            geom.computeBoundingBox();
            var zMax = geom.boundingBox.max.z;
            var xMax = geom.boundingBox.max.x;
            const ge = new THREE.BoxGeometry(PLOT, PLOT, PLOT);
            var mesh = new THREE.Mesh(geom, new THREE.MeshLambertMaterial({
                vertexColors: THREE.FaceColors,
                color: 0x666666,
                shading: THREE.SmoothShading
            }));
            mesh.material.side = THREE.DoubleSide;
            mesh.position.set(- SIZE / 2, SIZE / 2, DIST_FROM_CAMERA)
            mesh.rotateX(3.141592  / 2)
            mesh.name = 'valley'
            scene.add(mesh);

            box.name = 'highlight'
            scene.add(box)
        };

        const render = () => {
            //control.update()
            renderer.render(scene, camera)
            requestAnimationFrame(render)
        }
        render()
        root.addEventListener('click', event => {
            var raycaster = new THREE.Raycaster();
            let canvasBounds = renderer.context.canvas.getBoundingClientRect();
            let mouse = new THREE.Vector3();
            mouse.x = ( ( event.clientX - canvasBounds.left ) / ( canvasBounds.right - canvasBounds.left ) ) * 2 - 1;
            mouse.y =  - ( ( event.clientY - canvasBounds.top ) / ( canvasBounds.bottom - canvasBounds.top) ) * 2 + 1;
            raycaster.setFromCamera(
                new THREE.Vector2(
                    mouse.x,
                    mouse.y
                ),
                camera
            );

            // calculate objects intersecting the picking ray
            const intersects = raycaster.intersectObjects(scene.children).filter(i => i.object.name === 'valley')
            const vector = intersects[0].point
            const x = Math.floor(vector.x / PLOT)
            const y = Math.floor(vector.y / PLOT)
            let box = scene.children.filter(i => i.name === 'highlight')[0]
            box.position.set(x * PLOT, y * PLOT, DIST_FROM_CAMERA)

            let key = Plot.key_encode(x + 96, 96 - y)
            router.push('/plot/' + key)
            Plot.refresh(key)
        })

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
                box.position.set((x - 96) * PLOT, (96 - y) * PLOT, DIST_FROM_CAMERA);
        }
        return <Container style={{ height: '50rem' }} />
    }
}
