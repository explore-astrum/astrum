import * as React from 'react'
import Lego from '../lego'

export default Lego<React.InputHTMLAttributes<HTMLInputElement>>('input', {
    'className': 'u-focus-highlight',
    'weight-5': true,
    'fg-white': true,
    'weight-6': true,
    'border-1': true,
    'radius-max': true,
    'pad-v4': true,
    'pad-h6': true,
    style: {
        border: 'solid 2px white',
        width: '100%'
    }
})