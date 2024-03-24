const MEXC = require("./package/")
const fs = require('fs');
require('dotenv').config()

let asset = process.argv[2]

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

let result = client.accountInfo();

let foundAsset = result.balances.find((balance) => balance.asset == asset);

if (foundAsset) {
	console.log(foundAsset.free);
} else {
	console.log(-1);
}