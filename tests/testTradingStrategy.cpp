#include "../Classes/TradingStrategy.cpp"

#ifndef TEST_TRADINGSTRATEGY_CPP
#define TEST_TRADINGSTRATEGY_CPP

void testInit() {
	Positions positions;
	TradingStrategy tradingStrategy(positions);	
}

void testTradingStrategy() {
	testInit();

	Positions positions;
	TradingStrategy tradingStrategy(positions);	

	// double assetBalance = tradingStrategy.getAssetBalance("NOTREALCOINUSXASKHDG");
	// assert(assetBalance == -1);

	// assert(tradingStrategy.getAssetBalance("MX") != -1);

	// cout << tradingStrategy.getAssetBalance("BTC") << endl;

	// tradingStrategy.buy("BTCUSDC", 20);

	// double btcBalance = tradingStrategy.getAssetBalance("BTC");

	// cout << btcBalance << endl;
	
	// tradingStrategy.sellAsset("BTCUSDC");

	// btcBalance = tradingStrategy.getAssetBalance("BTC");

	// cout << btcBalance << endl;

	// assert(btcBalance == -1);

	// double balance = tradingStrategy.getAssetBalance("ETH");
	// assert(balance == 0);
	
	// double MXbalance = tradingStrategy.getAssetBalance("MX");
	// double BTCbalance = tradingStrategy.getAssetBalance("BTC");

	// assert(BTCbalance != -1);
	// assert(MXbalance != -1);

	bool getAssetBalanceResult = tradingStrategy.getAssetBalance("DOESNOTEXIST");

	assert(getAssetBalanceResult == 0);


	bool sellAssetResult = tradingStrategy.sellAsset("DOESNOTEXIST");
	assert(sellAssetResult == false);

	bool buyResult = tradingStrategy.buy("DOESNOTEXIST", 20);
	assert(buyResult == false);
	
	cout << "PASSES: TradingStrategy.cpp" << endl;
}


#endif