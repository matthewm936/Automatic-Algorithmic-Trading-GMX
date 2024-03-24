const MEXC = require("./package")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

let pairName = process.argv[2]

console.log(client.accountTradeList(pairName, {limit : 3}));
