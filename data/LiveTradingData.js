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

    fs.writeFile('/home/johnsmith/Trading/Algorithmic-Trading/data/prices.json', JSON.stringify(data, null, 4), (err) => {
        if (err) throw err;
        console.log('Data written to file');
        fs.appendFile('/home/johnsmith/Trading/Algorithmic-Trading/data/data log.txt', `Prices data grabbed from MEXC api and written to local file prices.json at ${new Date().toISOString()} : ${new Date().toLocaleString()}\n`, (err) => {
            if (err) throw err;
        });
    });
});