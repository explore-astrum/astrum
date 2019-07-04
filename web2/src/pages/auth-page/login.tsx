import * as React from 'react'
import { Container, Wrap, Anchor } from '../../components'
import * as Form from '../../components/form'
import Link from '../../structures/link'
import * as Session from '../../data/session'
import { kora, router } from '../../data/kora'

interface Props {

}

interface State {
    email?: string
    password?: string
}
export default class Login extends React.Component<Props, State> {
    state = {
        email: '',
        password: ''
    }
    render() {
        return (
            <Container flex justify-center align-center fill fg-white>
                <Wrap>
                    <Container flex justify-center>
                        <Container m-flex-5>
                            <Container size-8 weight-6>Welcome Back</Container>
                            <Container weight-6 mgn-t3 line-6>
                                Enter your credentials to access your account. Don't have an account? <Container inline fg-yellow><Link href="/auth/register">Register now →</Link></Container>
                            </Container>
                            <Container mgn-t8>
                                <Form.Input
                                    value={this.state.email}
                                    name="email"
                                    placeholder="Email"
                                    onChange={e => this.setState({
                                        email: e.target.value
                                    })} />
                                <Form.Input
                                    mgn-t6
                                    value={this.state.password}
                                    type="password"
                                    name="password"
                                    placeholder="Password"
                                    onChange={e => this.setState({
                                        password: e.target.value
                                    })} />
                            </Container>
                            <Container
                                mgn-t8
                                flex
                                justify-center
                                onClick={() => this.handle_login()}>
                                <Container
                                    fg-black
                                    fg-yellow
                                    cursor-pointer
                                    uppercase
                                    weight-8
                                    text-center
                                >Login →</Container>
                            </Container>
                        </Container>
                    </Container>
                </Wrap>
            </Container>
        )
    }
    private async handle_login() {
        try {
            const token = await kora.send<string>('auth.login', this.state, 1)
            await Session.upgrade(token)
            router.push('/plots')
        } catch (ex) {
            alert(ex)
        }
    }
}