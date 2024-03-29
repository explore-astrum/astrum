import { kora } from './kora'
import * as Hex from './hex'

export function info(key: string): PlotInfo {
    if (!key) schema({})
    return schema(kora.local_path<PlotInfo>(['plot:info', key]))
}

export async function refresh(key: string) {
    return await kora.query_path<PlotInfo>(['plot:info', key])
}

export function schema(plot: PlotInfo) {
    plot = plot || {}
    plot.prices = plot.prices || {}
    return plot
}

export function key_decode(key: string) {
    const x = key.substr(0, 4)
    const y = key.substr(4, 4)
    return {
        x: Hex.int16(x),
        y: Hex.int16(y),
    }
}

export function key_encode(x: number, y: number) {
    return ("0000" + x.toString(16)).substr(-4).toUpperCase() + ("0000" + y.toString(16)).substr(-4).toUpperCase()
}

export function activity(key: string) {
    return kora.local_values<PlotActivity>(['plot:activity', key])
}