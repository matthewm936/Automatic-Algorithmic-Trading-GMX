#ifndef TRADINGPAIRS_H
#define TRADINGPAIRS_H

#include <string>
#include <unordered_map>
#include "TradingPair.h"

class TradingPairs {
private:
public:
	std::unordered_map<std::string, TradingPair> pairs;
	double quoteVolumeUSDT = 0;
	double quoteVolumeUSDC = 0;
	double quoteVolumeBTC = 0;
	double quoteVolumeETH = 0;
	double quoteVolumeTUSD = 0;

	TradingPairs() {}

	void addPair(double price, std::string pairName, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol);

	TradingPair& getPair(std::string pairName);

	int getNumPairs() const;

	size_t size() const;
};

#endif 
