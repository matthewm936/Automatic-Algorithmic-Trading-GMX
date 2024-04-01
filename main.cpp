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

#include "Classes/Headers/Liquidity.h"
#include "Classes/Headers/Log.h"
#include "Classes/Headers/Positions.h"
#include "Classes/Headers/RunCommand.h"
#include "Classes/Headers/Time.h"
#include "Classes/Headers/TradingPair.h"
#include "Classes/Headers/TradingPairs.h"
#include "Classes/Headers/TradingStrategy.h"

using namespace std;

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
	
	Log::log("Main getting ticker information");
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

	Log::logPairs(tradingPairs);

	while(true) {
		time.start();
		Log::log("Running main loop");

		string prices = runCommand("node mexc-api/pair-prices.js");

		istringstream iss(prices);

		string pairName;
		double price;

		while (iss >> pairName >> price) {
			tradingPairs.pairs[pairName].updatePrice(price);

			tradingStrategy.trade(tradingPairs.pairs[pairName]);
		}

		Log::log("Main update prices and tradingStrategy.trades completed");

		time.end();
		Log::logPositions(positions);

		double sleepTimeMins = 1;
		double sleepTime = sleepTimeMins * 60 - time.getDuration();
		if (sleepTime < 0) sleepTime = 0; 

		Log::log("Sleeping for " + to_string(sleepTime) + " seconds");
		time.sleep(sleepTime);			
	}

	return 0;
}