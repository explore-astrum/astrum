const UINT_16_MAX = Math.pow(2, 16)
const INT_16_MAX = Math.pow(2, 15)

export function int16(data) {
    const result = parseInt(data, 16)
    if (result >= INT_16_MAX) return result - UINT_16_MAX
    return result
}

export function uint16(data) {
    return parseInt(data, 16)
}