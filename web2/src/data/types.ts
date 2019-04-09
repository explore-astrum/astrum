interface PlotInfo {
    key?: string
    owner?: string
    name?: string
}

interface Activity {
    type: 'plot.offer' | 'plot.list' | 'plot.sell'
    user: string
    target: string
    data: any
}