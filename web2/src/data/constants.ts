function parse() {
    // return ['ws://192.168.1.3:12000/socket', 'http://192.168.1.3:13000/rest']
    switch (process.env.NODE_ENV) {
        case 'development':
            // return ['wss://acs.ride-health.com/socket', 'https://acs.ride-health.com/rest']
            return ['ws://localhost:12000/socket', 'http://localhost:13000/rest']
        default:
            return [`wss://${location.host}/socket`, `https://${location.host}/rest`]
    }
}

const [SOCKET, REST] = parse()

export { SOCKET, REST }