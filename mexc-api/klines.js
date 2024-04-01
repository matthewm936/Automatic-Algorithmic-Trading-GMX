const MEXC = require("./package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

let pairName = process.argv[2]
let interval = process.argv[3]

async function klines() {
    try {
        let response = await client.klines(pairName, interval, 
        {
			limit: 10
        }) 
        console.log(response);
    } catch (error) {
        console.error("An error occurred: ", error);
        process.exit(1);
    }
}

klines();