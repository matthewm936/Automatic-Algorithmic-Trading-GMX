const MEXC = require("../MEXC JS SDK API/package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

logFile = '/home/johnsmith/Trading/Algorithmic-Trading/Trade/trade log.txt';

let pairName = process.argv[2]
let side = process.argv[3]
let stopPrice = process.argv[4]
let takeProfit = process.argv[5]

client.newOrder(pairName, side, "MARKET", {
    quantity: 1,
    timeInForce: "GTC",
    stopPrice: stopPrice,
	price: takeProfit,
	quoteOrderQty: 20
	
});
// .then((data) => {
// 	console.log(data);
// 	const writeFilePromise = new Promise((resolve, reject) => {
// 		fs.writeFile(logFile, JSON.stringify(data, null, 4), (err) => {
// 			if (err) reject(err);
// 			resolve();
// 		});
// 	});

// 	const path = require('path');

// 	writeFilePromise.then(() => {
// 		const filename = path.basename(__filename);
// 		const logMessage = [
// 			`Ran ${filename}`,
// 			`\tISO: ${new Date().toISOString()}`,
// 			`\tUnix: ${Date.now()}`
// 		].join('\n');

// 		fs.appendFile(logFile, `${logMessage}\n\n`, (err) => {
// 			if (err) console.error('Error writing to log file:', err);
// 		});
// 	}).catch((err) => {
// 		const errorMessage = `Error writing to file: ${err}`;
// 		fs.appendFile(logFile, `${errorMessage}\n\n`, (err) => {
// 			if (err) console.error('Error writing to log file:', err);
// 		});
// 	});
// }).catch((error) => {
//     // handle the error
// });