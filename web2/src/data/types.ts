interface PlotInfo {
    key?: string
    owner?: string
    name?: string
    prices?: {
        list?: number
        sold?: number
        offer?: number
    }
}

interface PlotActivity {
    key?: string
    type?: 'plot.list' | 'plot.sold'
    created?: number
    sender?: string
    data?: any
}

interface Activity {
    type: 'plot.offer' | 'plot.list' | 'plot.sell'
    user: string
    target: string
    data: any
}

interface RelicType {
    key?: string
    name?: string
}
