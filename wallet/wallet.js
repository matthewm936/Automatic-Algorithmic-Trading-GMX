const { Web3 } = require('web3');

//private RPC endpoint 
const web3 = new Web3('https://arbitrum-mainnet.infura.io/v3/e0177d37cfd24c28bc1f1ceb3e5c77a3'); 


// get the balance of an address
await web3.eth.getBalance('0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045');
// ↳ 114438180989009447638n

// get last block number
await web3.eth.getBlockNumber();
// ↳ 18849658n

// get the chain id of the current provider
await web3.eth.getChainId();
// ↳ 1n

// get the nonce of an address
await web3.eth.getTransactionCount('0x37826D8B5F4B175517A0f42c886f8Fca38C55Fe7');
// ↳ 7n

// get the current gas price
await web3.eth.getGasPrice();

// ↳ 23879160756n// this is my wallet address it should have aeth on arbitrum one
// .0003 aeth (rounding in metamask ig so its really .00025...)
// 0x03Ef040e2Bf78593e49d58447044C50B5b3475A1



// random generated wallet
// address: 0xE29998F2836a05C68CE3e984c2acB810fdC5eD0F
// priv key: 0xb40346c3cb3dab36980ec73476e9b719da22a2804b24acb9de0c711eae6d8a16


// GMX exchange route : https://arbiscan.io/address/0x7c68c7866a64fa2160f78eeae12217ffbf871fa8#writeContract

//Uniswap token address in mainnet
const address = '0x1f9840a85d5af5bf1d1762f925bdaddc4201f984'

//you can find the complete ABI in etherscan.io
const ABI = 
[
    {
      name: 'symbol',
      outputs: [{ type: 'string' }],
      type: 'function',
    },
    {
      name: 'totalSupply',
      outputs: [{ type: 'uint256' }],
      type: 'function',
    },
];

//instantiate the contract
const uniswapToken = new web3.eth.Contract(abi, address);
