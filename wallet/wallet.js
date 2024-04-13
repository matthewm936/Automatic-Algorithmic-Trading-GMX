const { ethers } = require('ethers');

// this is my wallet address it should have aeth on arbitrum one
// .0003 aeth
// 0x03Ef040e2Bf78593e49d58447044C50B5b3475A1

// const ethers = require('ethers')
const provider = new ethers.JsonRpcProvider(`https://arbitrum-mainnet.infura.io/v3/e0177d37cfd24c28bc1f1ceb3e5c77a3`);

console.log("ethers.js version: " + ethers.version);

(async function() {
    let blockNumber = await provider.getBlockNumber();
    console.log("Block Number: " + blockNumber);

    let balance = await provider.getBalance("0x03Ef040e2Bf78593e49d58447044C50B5b3475A1");

    console.log("Balance: " + ethers.formatEther(String(balance)) + " ether");
})();

