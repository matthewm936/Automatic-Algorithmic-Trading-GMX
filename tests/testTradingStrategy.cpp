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

	double assetBalance = tradingStrategy.getAssetBalance("NOTREALCOINUSXASKHDG");
	assert(assetBalance == -1);

	assert(tradingStrategy.getAssetBalance("MX") != -1);

	cout << assetBalance << endl;
	
	cout << "PASSES: TradingStrategy.cpp" << endl;
}


#endif