#include "../Classes/MovingAverage.cpp"
#include "../Classes/MovingAverageCrossover.cpp"
#include "../Classes/Liquidity.cpp"

#ifndef TESTLIQUIDITY_CPP
#define TESTLIQUIDITY_CPP

#include <cassert>

void testLiquidity() {
	Liquidity liquidity;

	double volumeMetric = liquidity.getBidAskQty("BTCUSDT");
	assert(volumeMetric != -1);

	double volume24hr = liquidity.get24hrVolume("BTCUSDT");
	assert(volume24hr != -1);

	double volume24hrNOTREAL = liquidity.get24hrVolume("NOTREALUSDTXX");
	assert(volume24hrNOTREAL == -1);

	double bidAskQP = liquidity.getBidAskQty("AAIUSDT");
	double bidAskQP1 = liquidity.getBidAskQty("NEXTGENUSDT");
	double bidAskQP2 = liquidity.getBidAskQty("THUSDT");
	double bidAskQP3 = liquidity.getBidAskQty("REKTUSDT");
	double bidAskQP4 = liquidity.getBidAskQty("2MOONUSDT");

	cout << bidAskQP << endl;
	cout << bidAskQP1 << endl;
	cout << bidAskQP2 << endl;
	cout << bidAskQP3 << endl;
	cout << bidAskQP4 << endl;

	cout << "PASSED values from api: Liquidity" << endl;
}

#endif