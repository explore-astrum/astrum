import * as React from 'react'
import { Anchor } from '../components'
import { router } from '../data/kora'

export default function Link(props: React.AnchorHTMLAttributes<HTMLAnchorElement>) {
    return <Anchor {...props} onClick={e => {
        router.push(props.href)
        e.preventDefault()
    }} />
}