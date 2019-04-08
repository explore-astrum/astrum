import * as React from 'react'
import { Container } from '../../components'
import Header from '../../structures/header'
import { kora } from '../../data/kora'

interface Props {

}

interface State {

}
export default class PlotPage extends React.Component<any, any> {
    render() {
        return (
            <Container>
                <Header />
            </Container>
        )
    }
}