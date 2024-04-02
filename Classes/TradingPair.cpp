#include <string>
#include <iostream>
#include <deque>
#include <fstream>
#include <vector>

#include "Headers/Time.h"
#include "Headers/Log.h"
#include "Headers/Liquidity.h"
#include "Headers/TradingPair.h"

std::chrono::time_point<std::chrono::system_clock> lastUpdate5min;
std::chrono::time_point<std::chrono::system_clock> lastUpdate30min;
std::chrono::time_point<std::chrono::system_clock> lastUpdate1hr;

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

TradingPair::TradingPair(double price, std::string pair, double ask, double bid, double askQ, double bidQ, double vol, double quoteVol) {
	pairName = pair;
	askPrice = ask;
	bidPrice = bid;
	askQty = askQ;
	bidQty = bidQ;
	volume = vol;
	quoteVolume = quoteVol;

	std::vector<std::string> quoteAssets = {"USDT", "USDC", "ETH", "BTC", "TUSD"};

	std::sort(quoteAssets.begin(), quoteAssets.end(), [](const std::string& a, const std::string& b) {
		return a.size() > b.size();
	});

	for (const auto& quote : quoteAssets) {
		if (pairName.size() > quote.size() && pairName.substr(pairName.size() - quote.size()) == quote) {
			quoteAsset = quote;
			baseAsset = pairName.substr(0, pairName.size() - quote.size());
			break;
		}
	}
}

double TradingPair::getCurrentPrice() const {
	return prices1minInterval.front();
}

void TradingPair::updatePrice(double price) {
	prices1minInterval.push_front(price);
	if (prices1minInterval.size() > 720) {
		prices1minInterval.pop_back();
	}

	auto now = std::chrono::system_clock::now();

	if (now - lastUpdate5min >= std::chrono::minutes(5)) {
		lastUpdate5min = now;
		prices5minInterval.push_front(price);
		if (prices5minInterval.size() > 288) { // Keep 24 hours of data
			prices5minInterval.pop_back();
		}
	}
	if (now - lastUpdate30min >= std::chrono::minutes(30)) {
		lastUpdate30min = now;
		prices30minInterval.push_front(price);
		if (prices30minInterval.size() > 96) { // Keep 2 days of data
			prices30minInterval.pop_back();
		}
	}

	if (now - lastUpdate1hr >= std::chrono::hours(1)) {
		lastUpdate1hr = now;
		prices1hrInterval.push_front(price);
		if (prices1hrInterval.size() > 240) { // Keep 10 days of data
			prices1hrInterval.pop_back();
		}
	}
}

