import * as React from 'react'
import { Container, Wrap } from '../../components/'
import Link from '../link'
import Lego from '../../components/lego'

interface Props { }
export default function Header(props: Props) {
    return (
        <Container fg-black pad-8 flex>
            <Container flex flex-grow>
                <Link href="/landing">
                    <Container style={{ fontFamily: 'Thruster', letterSpacing: '4px' }} weight-8 size-6>ASTRUM</Container>
                </Link>
            </Container>
            <Container flex weight-5>
                <Container mgn-l6>Plots</Container>
                <Container mgn-l6>Relics</Container>
            </Container>
        </Container>
    )
}
