const MEXC = require("./MEXC JS SDK API/package/")
require('dotenv').config()


const client = new MEXC.Spot()
client.config.apiKey = process.env.apiKey
client.config.apiSecret = process.env.apiSecret

const trades = client.trades("BTCUSDT", { limit: 5 })
console.log(trades)

const time = client.time()
console.log(time)

const res = client.newOrderTest("BTCUSDT", "BUY", "LIMIT", { quantity: 5, price: 10 })
console.log(res)

const singleRes = client.exchangeInfo({ symbol: "BTCUSDT" })
console.log(singleRes)
