// const MEXC = require("./mexc-api/package")
// const fs = require('fs');
// require('dotenv').config()

// const client = new MEXC.Spot()
// client.config.apiKey = process.env.apiKey;
// client.config.apiSecret = process.env.apiSecret;

// // let pairName = process.argv[2]
// // let side = process.argv[3]
// // let stopPrice = process.argv[4]
// // let takeProfit = process.argv[5]

// pairName = "NEXTGENUSDT"
// // pairName = "BTCUSDT"
// side = "SELL"


// // let reponse = client.newOrderTest(pairName, side, "MARKET", {
// //     quantity: 1,
// //     timeInForce: "GTC",
// //     stopPrice: stopPrice,
// // 	price: takeProfit,
// // 	quoteOrderQty: 20,
// //     newOrderRespType: "FULL"
// // });

// // console.log(reponse);
// // console.log(client.aggTrades(pairName, any = { limit: 5 } ))

// console.log(client.ticker24hr(pairName))
// console.log(client.bookTicker(pairName))

// console.log(client.klines(pairName, "1m", any = { limit: 20} ))
// console.log(client.depth(pairName, any = { limit: 20 }))

// // console.log(client.historicalTrades(pairName, any = { limit: 3 }))


// console.log(client.accountInfo());

// let reponse1 = client.newOrder(pairName, side, "MARKET", {
// 	quantity: 6.74451609,
//     timeInForce: "GTC",
// 	// quoteOrderQty: 6.74451609,
//     newOrderRespType: "FULL"
// });

// console.log(reponse1);

const axios = require('axios');

// Base URL of the API
const baseURL = 'https://api.mexc.com';

// Function to get the prices of all ticker symbols
function getPrices() {
	return axios.get(`${baseURL}/api/v3/ticker/24hr`)
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
		console.log(ticker);
	});
}).catch(error => {
	console.log('Error:', error.message);
}).finally(() => {
	process.exit();
});