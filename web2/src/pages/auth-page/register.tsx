import * as React from 'react'
import { Container, Wrap } from '../../components'
import * as Form from '../../components/form'
import Link from '../../structures/link'
import * as Session from '../../data/session'
import { kora, router } from '../../data/kora'

interface Props {

}

interface State {
    email?: string
    username?: string
    password?: string
}
export default class Register extends React.Component<Props, State> {
    state = {
        email: '',
        username: '',
        password: ''
    }
    render() {
        return (
            <Container flex justify-center align-center fill fg-white>
                <Wrap>
                    <Container flex justify-center>
                        <Container m-flex-5>
                            <Container size-8 weight-6>Register</Container>
                            <Container weight-6 mgn-t3 line-6>
                                Create an account to claim your piece of Astrum. Already have an account? <Container inline fg-yellow><Link href="/auth/login">Login now →</Link></Container>
                            </Container>
                            <Container mgn-t8>
                                <Form.Input
                                    value={this.state.email}
                                    placeholder="Email"
                                    autoComplete="email"
                                    onChange={e => this.setState({
                                        email: e.target.value
                                    })} />
                                <Form.Input
                                    mgn-t6
                                    value={this.state.username}
                                    autoComplete="username"
                                    placeholder="Username"
                                    onChange={e => this.setState({
                                        username: e.target.value
                                    })} />
                                <Form.Input
                                    mgn-t6
                                    value={this.state.password}
                                    autoComplete="password"
                                    type="password"
                                    placeholder="Password"
                                    onChange={e => this.setState({
                                        password: e.target.value
                                    })} />
                            </Container>
                            <Container
                                mgn-t8
                                flex
                                justify-center
                                onClick={() => this.handle_register()}>
                                <Container
                                    fg-black
                                    fg-yellow
                                    cursor-pointer
                                    uppercase
                                    weight-8
                                    text-center
                                >Register →</Container>
                            </Container>
                        </Container>
                    </Container>
                </Wrap>
            </Container>
        )
    }
    private async handle_register() {
        Session.lock(async () => {
            await kora.send('auth.register', this.state, 1)
            const token = await kora.send<string>('auth.login', this.state, 1)
            await Session.upgrade(token)
            router.push('/plots')
        })
    }
}