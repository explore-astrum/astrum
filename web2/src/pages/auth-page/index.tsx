import * as React from 'react'
import { Container } from '../../components'
import { kora, router } from '../../data/kora'

import Login from './login'
import Register from './register'

interface Props {

}

interface State {

}

export default class AuthPage extends React.Component<any, any> {
    render() {
        if (router.match_exact('/auth'))
            router.push('/auth/login')
        return (
            <Container>
                {router.match_exact('/auth/register') && <Register />}
                {router.match_exact('/auth/login') && <Login />}
            </Container>
        )
    }
}