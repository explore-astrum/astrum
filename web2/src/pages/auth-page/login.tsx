import * as React from 'react'
import { Container, Wrap } from '../../components'
import * as Form from '../../components/form'
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
            <Container flex justify-center align-center fill>
                <Wrap style={{ width: '100%' }}>
                    <Container flex justify-center>
                        <Container m-flex-5>
                            <Container size-6>Welcome Back</Container>
                            <Container mgn-t2 fg-gray>
                                Enter your credentials to access your account
                            </Container>
                            <Container mgn-t2>
                                <Form.Input
                                    pad-v2
                                    value={this.state.email}
                                    placeholder="Email"
                                    onChange={e => this.setState({
                                        email: e.target.value
                                    })} />
                                <Form.Input
                                    pad-v2
                                    value={this.state.password}
                                    onChange={e => this.setState({
                                        password: e.target.value
                                    })}
                                    placeholder="Password"
                                    type="password" />
                            </Container>
                            <Container
                                onClick={() => this.handle_login()}
                                mgn-t4
                                bg-black
                                text-center
                                pad-v4
                                size-3-5
                                radius-4
                                weight-5
                                cursor-pointer
                                fg-white>
                                Login
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
        } catch (ex) {
            alert(ex)
        }
    }
}