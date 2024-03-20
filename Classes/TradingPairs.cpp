#ifndef TradingPairs_CPP
#define TradingPairs_CPP

#include <string>
#include <iostream>
#include <unordered_map>

#include "TradingPair.cpp"

using namespace std;

class TradingPairs { 
	
private:
	unordered_map<string, TradingPair> pairs;

public:
	TradingPairs() {
		Log::log("init TradingPairs");
	}

	void addPair(double price, string pairName, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol) {
		TradingPair tradingPair(price, pairName, ask, bid, askQ, bidQ, vol, quoteVol);
		pairs[pairName] = tradingPair;
	}

	void updatePair(double price, std::string pairName) {
		auto it = pairs.find(pairName);
		if (it != pairs.end()) {
			it->second.updatePrice(price);
		} else {
			// Handle the case where the pairName is not found in the map
		}
	}

	int getNumPairs() {
		return pairs.size();
	}

	void print() {
		for (auto& pair : pairs) {
			cout << pair.first << " : " << pair.second.getCurrentPrice() << endl;
		}
		cout << "total pairs: " << getNumPairs() << endl;
	}

	size_t size() const {
		return pairs.size();
	}

};

#endif