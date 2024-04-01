#ifndef TRADINGPAIR_H
#define TRADINGPAIR_H

#include <string>
#include <deque>
#include <vector>
#include <chrono>

class TradingPair {
private:
	std::chrono::time_point<std::chrono::system_clock> lastUpdate5min;
	std::chrono::time_point<std::chrono::system_clock> lastUpdate30min;
	std::chrono::time_point<std::chrono::system_clock> lastUpdate1hr;

public:
	std::deque<double> prices1minInterval;
	std::deque<double> prices5minInterval;
	std::deque<double> prices30minInterval;
	std::deque<double> prices1hrInterval;

	std::string pairName;

	double askPrice;
	double bidPrice;

	double askQty;
	double bidQty;

	double volume;
	double quoteVolume;

	std::string quoteAsset;
	std::string baseAsset;

	TradingPair() {}

	TradingPair(double price, std::string pair, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol);

	double getCurrentPrice() const;

	void updatePrice(double price);
};

#endif // TRADINGPAIR_H
