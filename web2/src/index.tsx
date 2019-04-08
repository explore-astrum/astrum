import * as React from 'react'
import * as ReactDOM from 'react-dom'
import { kora, router } from './data/kora'

import RootPage from './pages/root-page'

const root = document.getElementById('root')
kora.onLocalChange.add(mut => console.log(kora.local_path([])))
kora.onLocalChange.add(() => (
    ReactDOM.render(<RootPage />, root)
))
