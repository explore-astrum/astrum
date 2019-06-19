import './data/types'
import * as React from 'react'
import * as ReactDOM from 'react-dom'
import { kora, router } from './data/kora'

import RootPage from './pages/root-page'

const root = document.getElementById('root')
// kora.onLocalChange.add(mut => console.log(kora.local_path([])))
function render() {
    const RootPage = require('./pages/root-page').default
    ReactDOM.render(<RootPage />, root)
}

kora.onLocalChange.add(handle_change)
let debounce
function handle_change(mut: Kora.Mutation) {
    if (debounce) clearTimeout(debounce)
    debounce = setTimeout(() => render(), 1000 / 60)
}

render()

if (module.hot) {
    module.hot.accept(render)
    module.hot.dispose(() => kora.onLocalChange.remove(handle_change))
}