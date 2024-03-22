#ifndef TradingPair_CPP
#define TradingPair_CPP

#include <string>
#include <iostream>
#include <deque>
#include <fstream>

#include "Time.cpp"
#include "Log.cpp"
#include "Trade.cpp"
#include "StrategyMomentum.cpp"
#include "Liquidity.cpp"

class TradingPair { 
	
private:
	deque<double> prices;
	string pairName;

	double askPrice;
	double bidPrice;

	double askQty;
	double bidQty;

	double volume;
	double quoteVolume;

	StrategyMomentum strategyMomentum;

	public:
		TradingPair() = default;
		TradingPair(double price, string pair, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol) {
			prices.push_front(price);
			pairName = pair;
			askPrice = ask;
			bidPrice = bid;
			askQty = askQ;
			bidQty = bidQ;
			volume = vol;
			quoteVolume = quoteVol;
		}

	double getCurrentPrice() {
		return prices.front();
	}

	string getPairName() {
		return pairName;
	}

	void updatePrice(double price) {
		prices.push_front(price);

		if (prices.size() > 60) {
			prices.pop_back();

			strategyMomentum.checkSignals(prices, pairName);
		}
	}

};

#endif 