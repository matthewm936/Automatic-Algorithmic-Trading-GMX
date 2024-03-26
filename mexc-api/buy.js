const MEXC = require("./package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

let pairName = process.argv[2]
let amount = process.argv[3]
let response;

try {
	let response = client.newOrder(pairName, "BUY", "MARKET", 
	{
		timeInForce: "GTC",
		quoteOrderQty: amount,
		newOrderRespType: "FULL"
	}) 
console.log(response);

} catch (error) {
	console.error("An error occurred: ", error);
	process.exit(1);
}

