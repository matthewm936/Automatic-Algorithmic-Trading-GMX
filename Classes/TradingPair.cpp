#ifndef TradingPair_CPP
#define TradingPair_CPP

#include <string>
#include <iostream>
#include <deque>
#include <fstream>

#include "Time.cpp"

using namespace std;

class TradingPair { 
	
private:
	deque<double> prices;
	string pairName;

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

		if(prices.size() > 60) {
			prices.pop_back();
			// if(prices[0] > prices[5] && prices[5] > prices[10] && prices[10] > prices[15]) {
			// 	double takeProfit = (prices[0] - prices[15]) / prices[15] * 100;
			// 	double stopLoss = takeProfit / 2;

			// 	system(("node /home/johnsmith/Trading/Algorithmic-Trading/Trade/mexc-trade.js " + getPairName() + " BUY " + to_string(stopLoss) + " " + to_string(takeProfit)).c_str());

			// }
			if(prices[0] > prices[15] && prices[15] > prices[30] && prices[30] > prices[45] && prices[0] > prices[45] * 1.20) {
				double takeProfit = (prices[0] - prices[45]) / prices[45] * 100;
				double stopLoss = takeProfit / 2;

				system(("node /home/johnsmith/Trading/Algorithmic-Trading/Trade/mexc-trade.js " + getPairName() + " BUY " + to_string(stopLoss) + " " + to_string(takeProfit)).c_str());

				// Time time;
				// time.sleep(1200);

				// system(("node /home/johnsmith/Trading/Algorithmic-Trading/Trade/mexc-trade.js " + getPairName() + " SELL " + to_string(stopLoss) + " " + to_string(takeProfit)).c_str());
			}
		}
	}

};

#endif 