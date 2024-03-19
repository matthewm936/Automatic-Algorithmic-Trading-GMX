#include "../Classes/MovingAverage.cpp"
#include "../Classes/MovingAverageCrossover.cpp"
#include "../Classes/Liquidity.cpp"

#ifndef TESTLIQUIDITY_CPP
#define TESTLIQUIDITY_CPP

#include <cassert>

void testLiquidity() {
	Liquidity liquidity;

	double volumeMetric = liquidity.getBidAskQty("BTCUSDT");
	cout << "bid ask qty * price: " << volumeMetric << endl;

	// double volume24hr = liquidity.get24hrVolume("BTCUSDT");
	// cout << "Volume: " << volume24hr << endl;

	cout << "Check values from api: Liquidity" << endl;
}

#endif