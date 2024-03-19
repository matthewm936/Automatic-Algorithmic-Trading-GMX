const MEXC = require("./package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

let pairName = process.argv[2]
let side = process.argv[3]
let stopPrice = process.argv[4]
let takeProfit = process.argv[5]

if(side == "BUY"){
    let reponse = client.newOrder(pairName, side, "MARKET", {
    quantity: 1,
    timeInForce: "GTC",
    stopPrice: stopPrice,
	price: takeProfit,
	quoteOrderQty: 20,
    newOrderRespType: "JSON"
});
}

if(side == "SELL") {
    let reponse = client.newOrder(pairName, side, "MARKET", {
    quantity: ,
    timeInForce: "GTC",
    newOrderRespType: "JSON"
});
}



console.log(reponse);