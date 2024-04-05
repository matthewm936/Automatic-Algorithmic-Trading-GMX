const axios = require('axios');
const fs = require('fs');

// Base URL of the API
const baseURL = 'https://arbitrum-api.gmxinfra.io';

function ping() {
	return axios.get(`${baseURL}/ping`)
		.then(response => {
			if (response.status >= 200 && response.status < 300) {
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
				console.log(response.data);

				return JSON.stringify(response.data);
			} else {
				throw new Error('Unexpected response: ' + JSON.stringify(response.data));
			}
		})
		.catch(error => {b
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

	// Added on April 2, 2024
	// At 5x leverage of 500 USDC Long on GMX
		// Token: Fee and Price Impact: Network Fee
		// BTC: $1.97 + $0.46
		// ETH: $2.89 + $0.46
		// SOL: $4.32 + $0.46
		// ARB: $5.62 + $0.46
		// LINK: +$2.01 + $0.46 PRICE IMPACT HELPING
		// DOGE: $1.07 + $0.46
		// AVAX: $4.20 + $0.46
		// XRP: $2.74 + $0.46
		// NEAR: $1.90 + $0.46
		// UNI: $5.42 + $0.46
		// LTC: $11.23 + $0.46
		// AAVE $1.22 + $0.46
		// BNB $2.08 + $0.46
		// OP: $1.17 + $0.46
		// ATOM: $4.35 + $0.46
	// looks like shorting has different price impact fees, the network fees seem to be the same

async function candles(limit, timeFrameParams) {
	const tokensOnGMX = ["BTC", "ETH", "SOL", "ARB", "LINK", "DOGE", "AVAX", "XRP", "NEAR", "UNI", "LTC", "AAVE", "BNB", "OP", "ATOM"]

	let data = {};

	for (let token of tokensOnGMX) {
		data[token] = {};
		let promises = timeFrameParams.map(timeFrame => {
			return axios.get(`${baseURL}/prices/candles?tokenSymbol=${token}&period=${timeFrame}&limit=${limit}`)
				.then(response => {
					if (response.status >= 200 && response.status < 300) {
						data[token][timeFrame] = response.data;
					} else {
						throw new Error('Unexpected response: ' + JSON.stringify(response.data));
					}
				})
				.catch(error => {
					console.error('Error:', error.message);
				});
		});

		// Wait for all promises to resolve
		await Promise.all(promises);
	}

	fs.writeFile('gmx-api/token-candles.json', JSON.stringify(data, null, 2), (err) => {
		if (err) {
			console.error('Error writing file:', err);
		}
	});
}


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

	ping().then(pingResult => {
		if (pingResult) {
			if (args[0] == 'tickers') {
				tickers().then(tickers => {
					console.log(tickers);
				});
			} else if (args[0] == 'latest') {
				console.log('Getting latest prices...');
				latest().then(latest => {
					console.log('Latest:', latest);
				});
			} else if (args[0] == 'tokens') {
				tokens().then(latest => {
					console.log(tokens);
				});
			} else if (args[0] == 'candles') {
				const limit = args[1];
				
				// Get timeframes from command line arguments
				const timeFrameParams = args.slice(2);

				candles(limit, timeFrameParams).then(() => console.log("candles"));
			}
		} else {
			console.error('API is not reachable.');
			process.exit(1); 
		}
	}).catch(error => {
		console.error('Error:', error.message);
		process.exit(1); 
	});
}

main(); 

