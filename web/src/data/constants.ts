if (location.protocol === 'https:')
	location.href = 'http://' + location.host
export const PROJECT_NAME = 'Untitled Project'
export const URL = process.env.NODE_ENV === 'development' ? 'ws://localhost:12000/socket' : `wss://${location.host}/socket`
export const OWNER = 'ARBVqozYmoXqNXPsmbZreeTu7sZRYV5dZn'