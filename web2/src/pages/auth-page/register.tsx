import * as React from 'react'
import { Container, Wrap } from '../../components'
import * as Form from '../../components/form'
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
            <Container flex justify-center align-center fill>
                <Wrap style={{ width: '100%' }}>
                    <Container flex justify-center>
                        <Container m-flex-5>
                            <Container size-6>Register</Container>
                            <Container mgn-t2 fg-gray>
                                Create an account to claim your piece of Astrum
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
                                    value={this.state.username}
                                    placeholder="Username"
                                    onChange={e => this.setState({
                                        username: e.target.value
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
                                onClick={() => this.handle_signup()}
                                mgn-t4
                                bg-black
                                text-center
                                pad-v4
                                size-3-5
                                radius-4
                                weight-5
                                cursor-pointer
                                fg-white>
                                Register
                            </Container>
                        </Container>
                    </Container>
                </Wrap>
            </Container>
        )
    }
    private async handle_signup() {
        kora.send('auth.register', this.state, 1)
            .then()
            .catch(ex => alert(ex))
    }
}