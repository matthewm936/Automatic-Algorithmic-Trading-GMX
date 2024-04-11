const { ethers } = require('ethers');

// Generate a new random mnemonic

let wallet = ethers.Wallet.createRandom();


console.log('Restored Ethereum Wallet:');
console.log('Address:', wallet.address);
console.log('Private Key:', wallet.privateKey);


let walletLoad = new ethers.Wallet(wallet.privateKey);

// Connect a wallet to mainnet
let provider = ethers.getDefaultProvider();
let walletWithProvider = new ethers.Wallet(wallet.privateKey, provider);