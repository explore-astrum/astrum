import * as React from 'react'
import { Container, Wrap } from '../../components/'
import Link from '../link'
import Lego from '../../components/lego'
import { router } from '../../data/kora';

interface Props { }
export default function Header(props: Props) {
    return (
        <Container fg-black pad-8 flex>
            <Container flex flex-grow>
                <Link href="/landing">
                    <Container style={{ fontFamily: 'Thruster', letterSpacing: '4px' }} weight-8 size-8 fg-yellow>ASTRUM</Container>
                </Link>
            </Container>
            <Container flex weight-8 fg-white uppercase align-center>
                <Link href="/plots">
                    <Container mgn-l6 fg-yellow={router.match_prefix('/plots')}>Plots</Container>
                </Link>
                <Link href="/relics">
                    <Container mgn-l6>Relics</Container>
                </Link>
            </Container>
        </Container>
    )
}
