const MEXC = require("./package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

let pairName = process.argv[2]
let amount = process.argv[3]

client.newOrder(pairName, "BUY", "MARKET", {
	timeInForce: "GTC",
	quoteOrderQty: amount,
	newOrderRespType: "FULL"
})
.then(response => {
	console.log(response);
})
.catch(error => {
	console.error(error);
	process.exit(1); 
});