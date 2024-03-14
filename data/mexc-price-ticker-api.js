const axios = require('axios');

// Base URL of the API
const baseURL = 'https://api.mexc.com';

// Function to get the price of a ticker symbol
function getPrice(symbol) {
	axios.get(`${baseURL}/api/v3/ticker/price`, {
		params: {
			symbol: symbol
		}
	})
	.then(response => {
		// If the response is an array, find the price of the requested symbol
		if (Array.isArray(response.data)) {
			const ticker = response.data.find(t => t.symbol === symbol);
			if (ticker) {
				console.log(ticker.price);
			} else {
				console.error('Symbol not found:', symbol);
			}
		}
		// If the response is an object, print the price
		else if (response.data && response.data.price) {
			console.log(response.data.price);
		}
		// If the response is neither an array nor an object, print an error
		else {
			console.error('Unexpected response:', response.data);
		}
	})
	.catch(error => {
		console.error('Error:', error);
	});
}

// Get the ticker symbol from the command-line arguments
const symbol = process.argv[2];
if (!symbol) {
	console.error('No ticker symbol provided');
	process.exit(1);
}

// Get the price of the ticker symbol
getPrice(symbol);