const axios = require('axios');

// Base URL of the API
const baseURL = 'https://arbitrum-api.gmxinfra.io';

function ping() {
	return axios.get(`${baseURL}/ping`)
		.then(response => {
			if (response.status >= 200 && response.status < 300) {
				console.log('Ping successful');
				return true;
			} else {
				throw new Error('Unexpected response: ' + JSON.stringify(response.data));
			}
		})
		.catch(error => {
			console.error('Error:', error.message);
			return false;
		});
}

function tickers() {
	return axios.get(`${baseURL}/prices/tickers`)
		.then(response => {
			if (response.status >= 200 && response.status < 300) {
				return response.data;
			} else {
				throw new Error('Unexpected response: ' + JSON.stringify(response.data));
			}
		})
		.catch(error => {
			console.error('Error:', error.message);
			return [];
		});
}

function latest() {
	return axios.get(`${baseURL}/signed_prices/latest`)
		.then(response => {
			if (response.status >= 200 && response.status < 300) {
				return response.data;
			} else {
				throw new Error('Unexpected response: ' + JSON.stringify(response.data));
			}
		})
		.catch(error => {
			console.error('Error:', error.message);
			return [];
		});
}

// needs required parameters of string and candle stick type (5m 15m 1h 4h 1d)
// function candleSticks() {

// }

function tokens() {
	return axios.get(`${baseURL}/tokens`)
		.then(response => {
			if (response.status >= 200 && response.status < 300) {
				return response.data;
			} else {
				throw new Error('Unexpected response: ' + JSON.stringify(response.data));
			}
		})
		.catch(error => {
			console.error('Error:', error.message);
			return [];
		});
}

function main() {
	let args = process.argv.slice(2);

	// check api connection
	ping().then(pingResult => {
		if (pingResult) {
			console.log('API is reachable.');

			if (args[0] == 'tickers') {
				tickers().then(tickers => {
					console.log('Tickers:', tickers);
				});
			} else if (args[0] == 'latest') {
				console.log('Getting latest prices...');
				latest().then(latest => {
					console.log('Latest:', latest);
				});
			} else if (args[0] == 'tokens') {
				console.log('Getting tokens...');
				tokens().then(latest => {
					console.log('Tokens:', tokens);
				});
			}
		} else {
			console.error('API is not reachable.');
			process.exit(1); // Exit with error code
		}
	}).catch(error => {
		console.error('Error:', error.message);
		process.exit(1); // Exit with error code
	});
}

main(); // Call the main function to execute the script

