import { kora } from './kora'

export function info(key: string): PlotInfo {
    if (!key) schema({})
    return schema(kora.local_path<PlotInfo>(['plot:info', key]))
}

export async function refresh(key: string) {
    return await kora.query_path<PlotInfo>(['plot:info', key])
}

export function schema(plot: PlotInfo) {
    plot = plot || {}
    return plot
}

export function key_decode(key: string) {
    const x = key.substr(0, 4)
    const y = key.substr(4, 4)
    return {
        x: Number.parseInt(x, 16),
        y: Number.parseInt(y, 16)
    }
}