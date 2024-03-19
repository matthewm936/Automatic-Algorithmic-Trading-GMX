const MEXC = require("./mexc-api/package")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

// let pairName = process.argv[2]
// let side = process.argv[3]
// let stopPrice = process.argv[4]
// let takeProfit = process.argv[5]

pairName = "MXUSDT"
side = "SELL"


// let reponse = client.newOrderTest(pairName, side, "MARKET", {
//     quantity: 1,
//     timeInForce: "GTC",
//     stopPrice: stopPrice,
// 	price: takeProfit,
// 	quoteOrderQty: 20,
//     newOrderRespType: "FULL"
// });

// console.log(reponse);

console.log(client.accountInfo());

// let reponse1 = client.newOrder(pairName, side, "MARKET", {
// 	quantity: 6.74451609,
//     timeInForce: "GTC",
// 	// quoteOrderQty: 6.74451609,
//     newOrderRespType: "FULL"
// });

// console.log(reponse1);