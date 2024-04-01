#include <string>
#include <iostream>
#include <unordered_map>

#include "Headers/TradingPair.h"
#include "Headers/TradingPairs.h"

void TradingPairs::addPair(double price, std::string pairName, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol) {
	TradingPair tradingPair(price, pairName, ask, bid, askQ, bidQ, vol, quoteVol);
	pairs[pairName] = tradingPair;

	if(pairs[pairName].quoteAsset == "USDT") {
		quoteVolumeUSDT += quoteVol;
	} else if(pairs[pairName].quoteAsset == "USDC") {
		quoteVolumeUSDC += quoteVol;
	} else if(pairs[pairName].quoteAsset == "BTC") {
		quoteVolumeBTC += quoteVol;
	} else if(pairs[pairName].quoteAsset == "ETH") {
		quoteVolumeETH += quoteVol;
	} else if(pairs[pairName].quoteAsset == "TUSD") {
		quoteVolumeUSDT += quoteVol;
	}
}

TradingPair& TradingPairs::getPair(std::string pairName) {
	return pairs[pairName];
}

int TradingPairs::getNumPairs() const {
	return pairs.size();
}

size_t TradingPairs::size() const {
	return pairs.size();
}
