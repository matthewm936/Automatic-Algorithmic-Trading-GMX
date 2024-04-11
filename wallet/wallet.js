const { ethers } = require('ethers');

// Generate a new random mnemonic
// let wallet = ethers.Wallet.createRandom();

// generated wall 0x0A412a090e3050861DA4072ebFF7C01a2AAa6331
// generated priv 0xd0f7873924d9f31ea715f6ad2dbbfe49d0b82f3bcbea9f8627d729b90f2cc675

// console.log('Restored Ethereum Wallet:');
// console.log('Address:', wallet.address);
// console.log('Private Key:', wallet.privateKey);
privateKey = "0xd0f7873924d9f31ea715f6ad2dbbfe49d0b82f3bcbea9f8627d729b90f2cc675"

// let provider = ethers.getDefaultProvider();
// console.log(provider);

// let wallet = new ethers.Wallet(privateKey, provider);
// console.log(wallet);
// console.log(typeof(wallet))

// console.log('Wallet connected to mainnet:', wallet.address);

// let blockTag = "latest";
// let balancePromise = wallet.getBalance(blockTag);

// balancePromise.then((balance) => {
//     console.log(balance);
// });

// const ethers = require('ethers')
const provider = new ethers.JsonRpcProvider(`https://arbitrum-mainnet.infura.io/v3/e0177d37cfd24c28bc1f1ceb3e5c77a3`);

(async function() {
    let blockNumber = await provider.getBlockNumber();
    console.log("Block Number: " + blockNumber);
})();// console.log(provider.getBalance("ethers.eth"))

