#ifndef TradingPair_CPP
#define TradingPair_CPP

#include <string>
#include <iostream>
#include <deque>

using namespace std;

class TradingPair { 
	
private:
	deque<double> prices;
	string pairName;

	public:
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
	}
};

#endif 