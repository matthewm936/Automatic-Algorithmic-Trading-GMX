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
extern string runCommand(const char* command);

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
				Log::LogWithTimestamp("TradingPair.cpp, buy signal from 10in15 from " + pairName);

				int VOLUME_THREASHOLD = 10000;
				if(Liquidity::get24hrVolume(pairName) < VOLUME_THREASHOLD) {
					Log::LogWithTimestamp("TradingPair.cpp, " + pairName + " has volume below" + to_string(VOLUME_THREASHOLD) + " not buying.");
					return;
				} else {
					string message = "TradingPair.cpp, " + pairName + " has volume above " + to_string(VOLUME_THREASHOLD) + " buying.";
					Log::LogWithTimestamp(message);

					Position newPosition = {pairName, price, price, -1};
					g_positions[pairName] = newPosition;
				}

				// string command = "node /mexc-api/buy.js " + pairName + " BUY " + " 20";
				// const char* cmd = command.c_str();
				// system(cmd);
			}

			if (strategyMomentum.buySignal20in45(prices)) {
				Log::LogWithTimestamp("TradingPair.cpp, buy signal from 20in45 from " + pairName);

				int VOLUME_THREASHOLD = 10000;
				if(Liquidity::get24hrVolume(pairName) < VOLUME_THREASHOLD) {
					Log::LogWithTimestamp("TradingPair.cpp, " + pairName + " has volume below" + to_string(VOLUME_THREASHOLD) + " not buying.");
					return;
				} else {
					string message = "TradingPair.cpp, " + pairName + " has volume above " + to_string(VOLUME_THREASHOLD) + " buying.";
					Log::LogWithTimestamp(message);

					Position newPosition = {pairName, price, price, -1};
					g_positions[pairName] = newPosition;
				}

				// string command = "node /mexc-api/buy.js " + pairName + " BUY " + " 20";
				// const char* cmd = command.c_str();
				// system(cmd);

			}
		}
	}

};

#endif 