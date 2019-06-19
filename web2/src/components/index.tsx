import * as React from 'react'
import Lego from './lego'

export const Container = Lego<React.HTMLAttributes<HTMLDivElement>>('div')
export const Span = Lego<React.HTMLAttributes<HTMLDivElement>>('span')
export const Image = Lego<React.ImgHTMLAttributes<HTMLImageElement>>('img', {
    style: {
        width: '100%',
        height: 'auto',
        display: 'block',
    },
})
export const Anchor = Lego<React.HTMLAttributes<HTMLAnchorElement>>('a')

export function Wrap(props: React.AnchorHTMLAttributes<HTMLDivElement>) {
    const { children, ...rest } = props
    return (
        <Container flex justify-center {...rest}>
            <Container style={{ width: '68rem', maxWidth: '100%' }}>
                {children}
            </Container>
        </Container>
    )
}