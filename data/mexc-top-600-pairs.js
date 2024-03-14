const MEXC = require("../MEXC JS SDK API/package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

logFile = '/home/johnsmith/Trading/Algorithmic-Trading/data/data log.txt';
pairListFile = '/home/johnsmith/Trading/Algorithmic-Trading/data/mexcpairs_filtered';
// Read the mexcpairs_filtered file

fs.readFile(pairListFile, 'utf8', (err, data) => {
    if (err) {
        console.error(err);
        return;
    }

    // Split the file content by line breaks to get an array of tickers
    const tickers = data.split('\n');

    // Create an array of promises for client.ticker24hr calls
    const promises = tickers.map(ticker => client.ticker24hr(ticker));

    // Wait for all promises to resolve
    Promise.all(promises).then(results => {
        // Create an object with ticker names as keys and results as values
        const data = {};
        for (let i = 0; i < tickers.length; i++) {
            data[tickers[i]] = results[i];
        }

        // Write the data object to the log file
        fs.writeFile(logFile, JSON.stringify(data, null, 2), err => {
            if (err) {
                console.error(err);
            }
        });
    });
});