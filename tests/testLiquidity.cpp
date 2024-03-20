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

	cout << "PASSED values from api: Liquidity" << endl;
}

#endif