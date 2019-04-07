import './styles.css'
import * as React from 'react'
import * as ReactDOM from 'react-dom'
import { kora } from './data/kora'

import LandingPage from './pages/landing-page'


const root = document.getElementById('root')
kora.onLocalChange.add(() => (
    ReactDOM.render(<LandingPage />, root)
))