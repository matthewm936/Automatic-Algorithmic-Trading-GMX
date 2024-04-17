const { IndexerClient, Network } = require("@dydxprotocol/v4-client-js");
// For the deployment by DYDX token holders, use below:
/*
import { IndexerConfig, ValidatorConfig } from "@dydxprotocol/v4-client-js";

const NETWORK: Network = new Network(
  'mainnet',
  new IndexerConfig(
    'https://indexer.dydx.trade',
    'wss://indexer.dydx.trade',
  ),
  new ValidatorConfig(
    'https://dydx-ops-rpc.kingnodes.com', // or other node URL
    'dydx-mainnet-1',
    {
      CHAINTOKEN_DENOM: 'adydx',
      CHAINTOKEN_DECIMALS: 18,
      USDC_DENOM: 'ibc/8E27BA2D5493AF5636760E354E46004562C46AB7EC0CC4C1CA14E9E20E2545B5',
      USDC_GAS_DENOM: 'uusdc',
      USDC_DECIMALS: 6,
    },
  ),
);
*/
const NETWORK = Network.testnet();

const client = new IndexerClient(NETWORK.indexerConfig);

client.markets.getPerpetualMarkets().then(response => {
  const markets = Object.keys(response.markets);
  console.log("Number of markets: ", markets.length);
});
// ticker is the market ticket, such as "BTC-USD"
// client.markets.getPerpetualMarketOrderbook("BTC-USD").then(response => {
//   const asks = response.asks;
//   const bids = response.bids;

//   console.log("Asks: ", asks);
//   console.log("Bids: ", bids);
// });