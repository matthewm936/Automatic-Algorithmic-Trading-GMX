// #include "../Classes/MovingAverage.cpp"
// #include "../Classes/MovingAverageCrossover.cpp"
// #include "../Classes/Liquidity.cpp"

// #ifndef TESTLIQUIDITY_CPP
// #define TESTLIQUIDITY_CPP

// #include <cassert>

// void testLiquidity() {

// 	double volumeMetric = Liquidity.getBidAskQty("BTCUSDT");
// 	assert(volumeMetric != -1);

// 	double volume24hr = Liquidity.get24hrVolume("BTCUSDT");
// 	assert(volume24hr != -1);

// 	double volume24hrNOTREAL = Liquidity.get24hrVolume("NOTREALUSDTXX");
// 	assert(volume24hrNOTREAL == -1);

// 	double bidAskQP = Liquidity.getBidAskQty("AAIUSDT");
// 	double bidAskQP1 = Liquidity.getBidAskQty("NEXTGENUSDT");
// 	double bidAskQP2 = Liquidity.getBidAskQty("THUSDT");
// 	double bidAskQP3 = Liquidity.getBidAskQty("REKTUSDT");
// 	double bidAskQP4 = Liquidity.getBidAskQty("2MOONUSDT");

// 	cout << bidAskQP << endl;
// 	cout << bidAskQP1 << endl;
// 	cout << bidAskQP2 << endl;
// 	cout << bidAskQP3 << endl;
// 	cout << bidAskQP4 << endl;

// 	cout << "PASSED values from api: Liquidity" << endl;
// }

// #endif