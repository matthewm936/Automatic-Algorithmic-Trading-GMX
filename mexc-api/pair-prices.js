const axios = require('axios');

// Base URL of the API
const baseURL = 'https://api.mexc.com';

// Function to get the prices of all ticker symbols
function getPrices() {
	return axios.get(`${baseURL}/api/v3/ticker/price`)
	.then(response => {
		// If the response is an array, return it
		if (Array.isArray(response.data)) {
			return response.data;
		}
		// If the response is an object, throw an error
		else {
			throw new Error('Unexpected response: ' + JSON.stringify(response.data));
		}
	});
}

// Get the prices of all ticker symbols and print them
getPrices().then(prices => {
	prices.forEach(ticker => {
		console.log(ticker.symbol, ticker.price);
	});
}).catch(error => {
	console.log('Error:', error.message);
}).finally(() => {
	process.exit();
});