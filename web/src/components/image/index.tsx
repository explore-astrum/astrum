import './styles.css'
import * as React from 'react'
import { Container } from '../container'
import { component } from '../theme'

export const Image = component<React.ImgHTMLAttributes<HTMLImageElement>>('img', 'image', {})

export function Icon(props) {
	const { src, ...rest } = props
	return (
		<Container cursor-pointer {...rest}>
			<Image
				style={{verticalAlign: 'top'}}
				width={18}
				src={`https://storage.googleapis.com/ridehealth-public/feather/${src}.svg`} />
		</Container>
	)
}