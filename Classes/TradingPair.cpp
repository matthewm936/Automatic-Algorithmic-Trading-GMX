#ifndef TradingPair_CPP
#define TradingPair_CPP

#include <string>
#include <iostream>
#include <deque>
#include <fstream>

#include "Time.cpp"
#include "StrategyMomentum.cpp"
#include "Log.cpp"
#include "Trade.cpp"
#include "Position.cpp"

using namespace std;

class TradingPair { 
	
private:
	deque<double> prices;
	string pairName;

	StrategyMomentum strategyMomentum;

	public:
		TradingPair() = default;
		TradingPair(double price, string pair) {
			prices.push_front(price);
			pairName = pair;
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

			if (strategyMomentum.buySignal20in45(prices)) {
				Log::LogWithTimestamp("TradingPair.cpp, buy signal from 20in45 from " + pairName);

				// string command = "node /mexc-api/buy.js " + pairName + " BUY " + " 20";
				// const char* cmd = command.c_str();
				// system(cmd);
				Position position = {pairName, price, price, -1};
			}
		}
	}

};

#endif 