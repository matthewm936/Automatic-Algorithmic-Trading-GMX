#ifndef TRADINGPAIRS_H
#define TRADINGPAIRS_H

#include <string>
#include <unordered_map>
#include "TradingPair.h"

class TradingPairs {
private:
public:
	std::unordered_map<std::string, TradingPair> pairs;

	TradingPairs() {}

	void addPair(double price, std::string pairName, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol);

	TradingPair& getPair(std::string pairName);

	int getNumPairs() const;

	size_t size() const;
};

#endif 
