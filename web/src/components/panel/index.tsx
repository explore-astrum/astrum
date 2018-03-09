import './styles.css'
import * as React from 'react'
import { Container } from '../../components/container'

import Kora from '../../data/kora'
import { component } from '../theme'
import { Icon } from '../image'

export const Header = component<React.DetailedHTMLProps<React.HTMLAttributes<HTMLDivElement>, HTMLDivElement>>(Container, '', {
	'flex': true,
	'weight-5': true,
	'bg-light-gray': true,
	'pad-h8': true,
	'pad-v6': true,
})

export const Root = component<React.DetailedHTMLProps<React.HTMLAttributes<HTMLDivElement>, HTMLDivElement>>(Container, 'panel', {
	'flex': true,
	'flex-column': true,
})