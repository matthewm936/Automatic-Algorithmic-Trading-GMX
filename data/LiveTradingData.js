const MEXC = require("../MEXC JS SDK API/package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

Promise.all([
    client.ticker24hr("BTCUSDT"),
    client.ticker24hr("ETHUSDT")
]).then(([btcData, ethData]) => {
    const data = {
        BTCUSDT: btcData,
        ETHUSDT: ethData
    };

    fs.writeFile('data/prices.json', JSON.stringify(data, null, 4), (err) => {
        if (err) throw err;
        console.log('Data written to file');
    });
}).catch(console.error);