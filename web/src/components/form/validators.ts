import * as moment from 'moment'

const DISPLAY_FORMAT = 'hh:mma'
export function time(input: string) {
	return moment(input, DISPLAY_FORMAT).isValid()
}

export function email(input: string) {
	return input.indexOf('@') > -1 && input.indexOf('.') > -1
}