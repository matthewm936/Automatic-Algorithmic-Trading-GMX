#ifndef TradingPair_CPP
#define TradingPair_CPP

#include <string>
#include <iostream>
#include <deque>
#include <fstream>

#include "Time.cpp"
#include "Log.cpp"
#include "Liquidity.cpp"

class TradingPair { 
	
private:
	std::chrono::time_point<std::chrono::system_clock> lastUpdate5min;
	std::chrono::time_point<std::chrono::system_clock> lastUpdate30min;
	std::chrono::time_point<std::chrono::system_clock> lastUpdate1hr;

	public:
	deque<double> prices1minInterval;
	deque<double> prices5minInterval;
	deque<double> prices30minInterval;
	deque<double> prices1hrInterval;

	string pairName;

	double askPrice;
	double bidPrice;

	double askQty;
	double bidQty;

	double volume;
	double quoteVolume;

	string quoteAsset;
	string baseAsset;

		TradingPair() {

		}
		
		TradingPair(double price, string pair, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol) {
			prices1minInterval.push_front(price);
			prices5minInterval.push_front(price);
			prices30minInterval.push_front(price);
			prices1hrInterval.push_front(price);

			pairName = pair;
			askPrice = ask;
			bidPrice = bid;
			askQty = askQ;
			bidQty = bidQ;
			volume = vol;
			quoteVolume = quoteVol;

			// Define possible quote assets
			std::vector<std::string> quoteAssets = {"USDT", "USDC", "ETH", "BTC", "TUSD"};

			// Extract quote asset and base asset from pairName
			for (const auto& quote : quoteAssets) {
				if (pairName.size() > quote.size() && pairName.substr(pairName.size() - quote.size()) == quote) {
					quoteAsset = quote;
					baseAsset = pairName.substr(0, pairName.size() - quote.size());
					break;
				}
			}
		}

	double getCurrentPrice() const {
		return prices1minInterval.front();
	}

	void updatePrice(double price) {
		prices1minInterval.push_front(price);
		if (prices1minInterval.size() > 720) {
			prices1minInterval.pop_back();
		}

		auto now = std::chrono::system_clock::now();

		if (now - lastUpdate5min >= std::chrono::minutes(5)) {
			lastUpdate5min = now;
			prices5minInterval.push_front(price);
			if (prices5minInterval.size() > 288) { // Keep 24 hours of data
				prices5minInterval.pop_back();
			}
		}
		if (now - lastUpdate30min >= std::chrono::minutes(30)) {
			lastUpdate30min = now;
			prices30minInterval.push_front(price);
			if (prices30minInterval.size() > 96) { // Keep 2 days of data
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