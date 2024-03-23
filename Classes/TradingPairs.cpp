#ifndef TradingPairs_CPP
#define TradingPairs_CPP

#include <string>
#include <iostream>
#include <unordered_map>

#include "TradingPair.cpp"

class TradingPairs { 
private:
public:
	unordered_map<string, TradingPair> pairs;

	TradingPairs() {
	}

	void addPair(double price, string pairName, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol) {
		TradingPair tradingPair(price, pairName, ask, bid, askQ, bidQ, vol, quoteVol);
		pairs[pairName] = tradingPair;
	}

	TradingPair& getPair(std::string pairName) {
		return pairs[pairName];
	}

	int getNumPairs() const {
		return pairs.size();
	}

	size_t size() const {
		return pairs.size();
	}

};


#endif