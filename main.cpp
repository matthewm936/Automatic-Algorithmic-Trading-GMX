#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdio>
#include <map>
#include <string>
#include <locale>
#include <sstream>

#include "nlohmann/json.hpp"

#include "Classes/TradingPairs.cpp"
#include "Classes/Time.cpp"
#include "Classes/Log.cpp"
#include "Classes/Liquidity.cpp"
#include "Classes/TradingStrategy.cpp"

double getDoubleFromJson(nlohmann::json::iterator& it, const string& key) {
	string str = it->at(key).get<string>();
	return stod(str);
}

std::string formatWithCommas(int value) {
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

int main() {
	Time time;
	Log::clearLogFile();

	Log::LogWithTimestamp("MAIN.cpp Started");

	TradingPairs tradingPairs;

	Positions positions;

	TradingStrategy tradingStrategy(positions, tradingPairs);
	
	runCommand("node mexc-api/ticker24hrALL.js");

	ifstream i("prices.json");
	nlohmann::json j;
	i >> j;

	double totalQuoteVolume = 0;

	int quoteVolCountMore50k = 0;
	int quoteVolCountMore150k = 0;
	int quoteVolCountMore1M = 0;
	

	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		std::string symbol = it->at("symbol").get<std::string>();
		double lastPrice = getDoubleFromJson(it, "lastPrice");
		double askPrice = getDoubleFromJson(it, "askPrice");
		double bidPrice = getDoubleFromJson(it, "bidPrice");
		double askQty = getDoubleFromJson(it, "askQty");
		double bidQty = getDoubleFromJson(it, "bidQty");
		double volume = getDoubleFromJson(it, "volume");
		double quoteVolume = getDoubleFromJson(it, "quoteVolume");

		tradingPairs.addPair(lastPrice, symbol, askPrice, bidPrice, askQty, bidQty, volume, quoteVolume);

		totalQuoteVolume += quoteVolume;

		if(quoteVolume > 50000) {
			quoteVolCountMore50k++;
		}
		if(quoteVolume > 150000) {
			quoteVolCountMore150k++;
		}
		if(quoteVolume > 1000000) {
			quoteVolCountMore1M++;
			Log::log("Pair " + symbol + " has quote volume > 1M: " + formatWithCommas(quoteVolume));
		}
	}

	Log::log("Main.cpp pairs information:");
	Log::log("       total pairs: " + to_string(tradingPairs.getNumPairs()));
	Log::log("       pairs with quote volume > 50k: " + to_string(quoteVolCountMore50k));
	Log::log("       pairs with quote volume > 150k: " + to_string(quoteVolCountMore150k));
	Log::log("       pairs with quote volume > 1M: " + to_string(quoteVolCountMore1M));

	Log::log("       total quote volume: " + formatWithCommas(totalQuoteVolume));
	Log::log("       average quote volume: " + formatWithCommas(totalQuoteVolume / tradingPairs.size()));

	while(true) {
		time.start();

		string prices = runCommand("node mexc-api/pair-prices.js");

		istringstream iss(prices);

		string pairName;
		double price;

		while (iss >> pairName >> price) {
			tradingPairs.pairs[pairName].updatePrice(price);

			tradingStrategy.trade(tradingPairs.pairs[pairName]);
		}

		time.end();
		Log::log("total positions: " + to_string(positions.size()));

		double sleepTimeMins = 1;
		double sleepTime = sleepTimeMins * 60 - time.getDuration();
		if (sleepTime < 0) sleepTime = 0; 

		time.sleep(sleepTime);			
	}

	return 0;
}