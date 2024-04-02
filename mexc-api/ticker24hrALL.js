const axios = require('axios');

const baseURL = 'https://api.mexc.com';

function getPrices() {
	return axios.get(`${baseURL}/api/v3/ticker/24hr`)
	.then(response => {
		if (response.status >= 200 && response.status < 300) {
			return JSON.stringify(response.data);
		} else {
			throw new Error('Unexpected response: ' + JSON.stringify(response.data));
		}
	});
}

const fs = require('fs');

getPrices().then(result => {
    console.log(result);
});