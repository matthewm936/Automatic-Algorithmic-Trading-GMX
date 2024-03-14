const MEXC = require("../MEXC JS SDK API/package/")
const fs = require('fs');
require('dotenv').config()

const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey;
client.config.apiSecret = process.env.apiSecret;

logFile = '/home/johnsmith/Trading/Algorithmic-Trading/data/data log.txt';

Promise.all([
    client.ticker24hr("BTCUSDT"),
    client.ticker24hr("ETHUSDT"),
    client.ticker24hr("GALAUSDT"),
    client.ticker24hr("RBNUSDT"),
    client.ticker24hr("AEVOUSDT"),
    client.ticker24hr("MTRMUSDT"),
]).then(([btcData, ethData, galaData, rbnData, aevoData, mtrmData]) => {
    const data = {
        BTCUSDT: btcData,
        ETHUSDT: ethData,
        GALAUSDT: galaData,
        RBNUSDT: rbnData,
        AEVOUSDT: aevoData,
        MTRMUSDT: mtrmData,
    };

	const writeFilePromise = new Promise((resolve, reject) => {
		fs.writeFile('/home/johnsmith/Trading/Algorithmic-Trading/data/prices.json', JSON.stringify(data, null, 4), (err) => {
			if (err) reject(err);
			resolve();
		});
	});

	const path = require('path');

	writeFilePromise.then(() => {
		const filename = path.basename(__filename);
		const logMessage = [
			`Ran ${filename}`,
			`\tISO: ${new Date().toISOString()}`,
			`\tUnix: ${Date.now()}`
		].join('\n');

		fs.appendFile(logFile, `${logMessage}\n\n`, (err) => {
			if (err) console.error('Error writing to log file:', err);
		});
	}).catch((err) => {
		const errorMessage = `Error writing to file: ${err}`;
		fs.appendFile(logFile, `${errorMessage}\n\n`, (err) => {
			if (err) console.error('Error writing to log file:', err);
		});
	});
});