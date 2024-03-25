#include <cstdlib>


#include "../Classes/TradingStrategy.cpp"

#ifndef TEST_TRADINGSTRATEGY_CPP
#define TEST_TRADINGSTRATEGY_CPP

void testInit() {
	Positions positions;
	TradingStrategy tradingStrategy(positions);	
}

class TradingStrategyTest {
public:
	void consistentMovement() {
		Positions positions;
		TradingStrategy strategy(positions);
		TradingPair pair = TradingPair(99.0, "BTCUSDT", 50.0, 99.0, 10.0, 10.0, 1000.0, 100000.0);

		assert(pair.getCurrentPrice() == 99.0);

		srand(1); // seed the random number generator

		for (int i = 0; i < 1000; i++) {
			double randomPrice = 100.0 + (rand() % 100) - 1 + i; // generate a random price between 50 and 150
			pair.updatePrice(randomPrice);
			cout << "Price: " << randomPrice << endl;
		}

		int result = strategy.consistentMovement(pair, 240, 0.7);
		assert(result == 1);
	}
};

void testGetBalanceErrors() {
	Positions positions;
	TradingStrategy tradingStrategy(positions);	

	bool getAssetBalanceResult = tradingStrategy.getAssetBalance("DOESNOTEXIST");

	assert(getAssetBalanceResult == 0);


	string result = runCommand((string("node mexc-api/sell.js FAKEASSETFAKEAAA 20").c_str()));
	assert(result == "error");

	bool sellAssetResult = tradingStrategy.sellAsset("DOESNOTEXIST");
	assert(sellAssetResult == false);

	bool buyResult = tradingStrategy.buy("DOESNOTEXIST", 20);
	assert(buyResult == false);
	
	cout << "PASSES: TradingStrategy.cpp" << endl;
}

void testTradingStrategy() {
	testInit();
	testGetBalanceErrors();

	TradingStrategyTest test;
	test.consistentMovement();

}

#endif