const MEXC = require("./package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

let pairName = process.argv[2]
let stopLoss = String(process.argv[3])
let takeProfit = String(process.argv[4])
let amount = String(process.argv[5])

let response = client.newOrder(pairName, "SELL", "MARKET", 
{
	stopPrice: stopLoss,
    price: stopLoss - 1,
	quantity: amount,
	newOrderRespType: "FULL",
	timeInForce: "GTC"
}) 
console.log(response);

// let responseT = client.newOrder(pairName, "SELL", "MARKET", 
// {
//     stopPrice: takeProfit,
// 	// price: takeProfit,
// 	quantity: amount,
// 	newOrderRespType: "FULL",
// 	timeInForce: "GTC"
// }) 
// console.log(responseT);