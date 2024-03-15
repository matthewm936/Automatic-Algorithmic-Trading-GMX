#ifndef TradingPair_CPP
#define TradingPair_CPP

#include <string>
#include <iostream>
#include <deque>
#include <fstream>

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
		ofstream outFile("/home/johnsmith/Trading/Algorithmic-Trading/Classes/trading pair log.txt", ios::app);

		if(prices.size() > 60) {
			prices.pop_back();
			if(price[0] > price[5] > price[10] > price[15]) {
				outFile << getPairName() << " is up 3 consecutive periods of 5 mins for %" << 
				(price[0]/price[15]) / price[15] <<
				endl;
			}
			if(price[0] > price[15] > price[30] > price[45]) {
				outFile << getPairName() << " is up 3 consecutive periods of 15 mins for %" << 
				(price[0]/price[45]) / price[45] <<
				endl;
			}
			
		}
	}

};

#endif 