#include "../Classes/Trade.cpp"

#ifndef TESTTRADE_CPP
#define TESTTRADE_CPP

void testInit() {
	Trade trade = Trade();
	assert(trade.getTrades() == 0);
	assert(trade.getShorts() == 0);
	assert(trade.getLongs() == 0);
	assert(trade.getShortsClosed() == 0);
	assert(trade.getLongsClosed() == 0);
	assert(trade.getEntryPrice() == -1);
}

void testTrade() {
	testInit();

	Trade trade = Trade();

	trade.enterLong(100);
	assert(trade.getEntryPrice() == 100);
	assert(trade.getTrades() == 1);
	assert(trade.getLongs() == 1);
	assert(trade.getShorts() == 0);
	
	double profit = trade.exitLong(110);
	assert(profit == 0.10);
	assert(trade.getLongsClosed() == 1);
	assert(trade.getTrades() == 1);
	assert(trade.getLongs() == 1);
	assert(trade.getShorts() == 0);
	
	cout << "PASSES: trade.cpp" << endl;
}


#endif