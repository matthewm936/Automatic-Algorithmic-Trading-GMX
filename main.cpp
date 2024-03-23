#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdio>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

#include "Classes/TradingPairs.cpp"
#include "Classes/Time.cpp"
#include "Classes/Log.cpp"
#include "Classes/Liquidity.cpp"
#include "Classes/StrategyMomentum.cpp"

double getDoubleFromJson(nlohmann::json::iterator& it, const string& key) {
	string str = it->at(key).get<string>();
	return stod(str);
}

int main() {
	Time time;
	Log::LogWithTimestamp("MAIN.cpp Started");

	TradingPairs tradingPairs;
	StrategyMomentum strategyMomentum;
	Positions positions;
	
	runCommand("node mexc-api/ticker24hrALL.js");

	ifstream i("prices.json");
	nlohmann::json j;
	i >> j;

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
	}

	Log::log("main.cpp, TradingPairs pairs has " + to_string(tradingPairs.getNumPairs()) + " of trading pairs");

	while(true) {
		time.start();

		string prices = runCommand("node mexc-api/pair-prices.js");

		istringstream iss(prices);

		string pairName;
		double price;

		while (iss >> pairName >> price) {
			tradingPairs.pairs[pairName].updatePrice(price);

			// vector<string> tradeSignals = strategMomentum.checkSignals(tradingPairs.pairs[pairName]);	
			cout << "pairName: " << pairName << " price: " << price << endl;
		}

		time.end();
		Log::log("total positions: " + to_string(positions.size()));

		double sleepTimeMins = 1;
		double sleepTime = sleepTimeMins * 60 - time.getDuration();
		if (sleepTime < 0) sleepTime = 0; 
		time.sleep(sleepTime);
			
		Log::log("main.cpp exec time: " + to_string(time.getDuration()) + "s" + " so sleeping for " + to_string(sleepTime) + "s");

	}

	return 0;
}