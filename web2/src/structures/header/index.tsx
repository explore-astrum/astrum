import * as React from 'react'
import { Container, Wrap } from '../../components/'
import Link from '../link'
import Lego from '../../components/lego'
import { router, kora } from '../../data/kora'
import * as Session from '../../data/session'

interface Props { }
export default function Header(props: Props) {
    return (
        <Container fg-black pad-8 flex>
            <Container flex flex-grow>
                <Link href="/">
                    <Container style={{ fontFamily: 'Thruster', letterSpacing: '4px' }} weight-8 size-8 fg-yellow>ASTRUM</Container>
                </Link>
            </Container>
            <Container flex weight-8 fg-white uppercase align-center>
                <Link href="/plots">
                    <Container mgn-l6 fg-yellow={router.match_prefix('/plots')}>Plots</Container>
                </Link>
                <Link href="/relics" mgn-l6>
                    <Container>Relics</Container>
                </Link>
                {
                    Session.user() &&
                    <React.Fragment>
                        <Container mgn-l6>|</Container>
                        {/* <Link href="">
                            <Container mgn-l6>Hello thdxr</Container>
                        </Link> */}
                        <Container cursor-pointer mgn-l6 onClick={() => {
                            localStorage.removeItem('token')
                            kora.merge(['session', 'user'], false)
                            router.push('/auth/login')
                        }}>Logout</Container>
                    </React.Fragment>
                }
                {
                    Session.user() == false &&
                    <React.Fragment>
                        <Container mgn-l6>|</Container>
                        <Link href="/auth/login" mgn-l6>
                            <Container>Login</Container>
                        </Link>
                    </React.Fragment>
                }
            </Container>
        </Container>
    )
}
