#ifndef TradingPair_CPP
#define TradingPair_CPP

#include <string>
#include <iostream>
#include <deque>
#include <fstream>

#include "Time.cpp"
#include "Log.cpp"
#include "Trade.cpp"
#include "Liquidity.cpp"

class TradingPair { 
	
private:
	std::chrono::time_point<std::chrono::system_clock> lastUpdate30min;
	std::chrono::time_point<std::chrono::system_clock> lastUpdate1hr;

	public:
	deque<double> prices1minInterval;
	deque<double> prices30minInterval;
	deque<double> prices1hrInterval;

	string pairName;

	double askPrice;
	double bidPrice;

	double askQty;
	double bidQty;

	double volume;
	double quoteVolume;

		TradingPair() = default;
		TradingPair(double price, string pair, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol) {
			prices1minInterval.push_front(price);
			prices30minInterval.push_front(price);
			prices1hrInterval.push_front(price);

			pairName = pair;
			askPrice = ask;
			bidPrice = bid;
			askQty = askQ;
			bidQty = bidQ;
			volume = vol;
			quoteVolume = quoteVol;
		}

	double getCurrentPrice() {
		return prices1minInterval.front();
	}

	void updatePrice(double price) {
		prices1minInterval.push_front(price);
		if (prices1minInterval.size() > 60) {
			prices1minInterval.pop_back();
		}

		auto now = std::chrono::system_clock::now();
		if (now - lastUpdate30min >= std::chrono::minutes(30)) {
			lastUpdate30min = now;
			prices30minInterval.push_front(price);
			if (prices30minInterval.size() > 48) { // Keep 24 hours of data
				prices30minInterval.pop_back();
			}
		}

		if (now - lastUpdate1hr >= std::chrono::hours(1)) {
			lastUpdate1hr = now;
			prices1hrInterval.push_front(price);
			if (prices1hrInterval.size() > 240) { // Keep 10 days of data
				prices1hrInterval.pop_back();
			}
		}
	}

};

#endif 