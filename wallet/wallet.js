const { ethers } = require('ethers');

// this is my wallet address it should have aeth on arbitrum one
// .0003 aeth (rounding in metamask ig so its really .00025...)
// 0x03Ef040e2Bf78593e49d58447044C50B5b3475A1

// const ethers = require('ethers')
const provider = new ethers.JsonRpcProvider(`https://arbitrum-mainnet.infura.io/v3/e0177d37cfd24c28bc1f1ceb3e5c77a3`);

console.log("ethers.js version: " + ethers.version);

(async function() {
    let blockNumber = await provider.getBlockNumber();
    console.log("Block Number: " + blockNumber);

    let balance = await provider.getBalance("0x03Ef040e2Bf78593e49d58447044C50B5b3475A1");

    console.log("Balance: " + ethers.formatEther(String(balance)) + " ether");

    console.log("Network: " + (await provider.getNetwork()).name);
})();

let generatedWallet = ethers.Wallet.createRandom();
console.log("Generated Wallet: " + generatedWallet.address);
console.log("Generated Wallet Private Key: " + generatedWallet.privateKey);

// random generated wallet
// address: 0xE29998F2836a05C68CE3e984c2acB810fdC5eD0F
// priv key: 0xb40346c3cb3dab36980ec73476e9b719da22a2804b24acb9de0c711eae6d8a16
