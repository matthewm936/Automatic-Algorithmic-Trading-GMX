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
#include "Liquidity.cpp"

using namespace std;

extern map<string, Position> g_positions;

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

			if(strategyMomentum.buySignal10in15(prices)) {
				Log::LogWithTimestamp("| BUY signal 10in15 from " + pairName + " volume:" + to_string(volume) + " quoteVolume:" + to_string(quoteVolume));

				int VOLUME_THREASHOLD = 500;

				double bidAskQP = Liquidity::getBidAskQty(pairName);
				if(bidAskQP < VOLUME_THREASHOLD) {
					Log::LogWithTimestamp("TradingPair.cpp, " + pairName + " has volume below" + to_string(VOLUME_THREASHOLD) + " not buying.");
					return;
				} else {
					string message = "TradingPair.cpp, " + pairName + " has bidAskQP " + to_string(bidAskQP) + " buying at" + to_string(price);
					Log::LogWithTimestamp(message);

					if (g_positions.find(pairName) == g_positions.end()) {
						Position newPosition = {pairName, price, price, -1, time(nullptr)};
						g_positions[pairName] = newPosition;
					}
				}
			}

			if (strategyMomentum.buySignal20in45(prices)) {
				Log::LogWithTimestamp("| BUY signal 20in45 from " + pairName + " volume:" + to_string(volume) + " quoteVolume:" +  to_string(quoteVolume));

				int VOLUME_THREASHOLD = 50000;

				double bidAskQP = Liquidity::getBidAskQty(pairName);
				if(bidAskQP < VOLUME_THREASHOLD) {
					Log::LogWithTimestamp("TradingPair.cpp, " + pairName + " has volume below" + to_string(VOLUME_THREASHOLD) + " not buying.");
					return;
				} else {
					string message = "TradingPair.cpp, " + pairName + " has bidAskQP " + to_string(bidAskQP) + " buying.";
					Log::LogWithTimestamp(message);

					if (g_positions.find(pairName) == g_positions.end()) {
						Position newPosition = {pairName, price, price, -1, time(nullptr)};
						g_positions[pairName] = newPosition;
					}
				}
			}
		}
	}

};

#endif 