const MEXC = require("./package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

// console.log(client.ping())
// // client.accountInfo()
// console.log(client.accountInfo());

// console.log(client.newOrder("TITANUSDT", "SELL", "MARKET", {quantity: 671907545}))
// console.log(client.allOrders("SOBERUSDT"));
// )

console.log(client.accountInfo());
