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

#include "Classes/Headers/Log.h"
#include "Classes/Headers/Positions.h"
#include "Classes/Headers/RunCommand.h"
#include "Classes/Headers/Time.h"
#include "Classes/Headers/TradingPair.h"
#include "Classes/Headers/TradingPairs.h"
#include "Classes/Headers/TradingStrategy.h"

#include "Classes/Token.cpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

using json = nlohmann::json;

double getDoubleFromJson(nlohmann::json::iterator& it, const string& key) {
	string str = it->at(key).get<string>();
	return stod(str);
}

string formatWithCommas(int value) {
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

int main() {
	// setup logging file system
	Log::clearLogFiles();
	Log::LogWithTimestamp("MAIN.cpp Started");

	// setup objects
	Time time;

	TradingPairs MEXC_tradingPairs;

	unordered_map<string, Token> GMX_tokens;

	Positions positions;
	TradingStrategy MEXC_tradingStrategy(positions, MEXC_tradingPairs);

	// setup trading pairs
	runCommand("node gmx-api/gmx-rest-endpoints.js candles");

	std::ifstream gmx_token_candles_data("gmx-api/token-candles.json");
	nlohmann::json j;
	gmx_token_candles_data >> j;

	// Iterate over all tokens in the JSON
	for (nlohmann::json::iterator tokenIt = j.begin(); tokenIt != j.end(); ++tokenIt) {
		// Create a new Token object for each token in the JSON
		Token token(tokenIt.key());

		// Iterate over all time frames for the current token
		for (nlohmann::json::iterator timeframeIt = tokenIt->begin(); timeframeIt != tokenIt->end(); ++timeframeIt) {
			// Create a new Candlesticks object for each time frame
			Candlesticks candlesticks(timeframeIt.key());

			// Get the array of candles for the current time frame
			nlohmann::json candlesArray = timeframeIt->at("candles");

			// Iterate over all candles in the array
			for (nlohmann::json::iterator candleIt = candlesArray.begin(); candleIt != candlesArray.end(); ++candleIt) {
				// Create a new Candle object for each candle in the array
				Candle candle((*candleIt)[0].get<double>(), (*candleIt)[1].get<double>(), (*candleIt)[2].get<double>(), (*candleIt)[3].get<double>(), (*candleIt)[4].get<double>());

				// Add the Candle object to the Candlesticks object
				candlesticks.addCandle(candle);
			}

			// Add the Candlesticks object to the Token object
			token.addCandlesticks(candlesticks);
		}

		// Now you have a Token object with all its Candlesticks and Candles set up
		// You can add the Token object to a collection, use it directly, etc.
	}


	// string MEXC_tickers = runCommand("node mexc-api/ticker24hrALL.js");

	// std::ifstream i("prices.json");
	// nlohmann::json j;
	// i >> j;

	// for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
	// 	string symbol = it->at("symbol").get<std::string>();
	// 	double lastPrice = getDoubleFromJson(it, "lastPrice");
	// 	double askPrice = getDoubleFromJson(it, "askPrice");
	// 	double bidPrice = getDoubleFromJson(it, "bidPrice");
	// 	double askQty = getDoubleFromJson(it, "askQty");
	// 	double bidQty = getDoubleFromJson(it, "bidQty");
	// 	double volume = getDoubleFromJson(it, "volume");
	// 	double quoteVolume = getDoubleFromJson(it, "quoteVolume");

	// 	MEXC_tradingPairs.addPair(lastPrice, symbol, askPrice, bidPrice, askQty, bidQty, volume, quoteVolume);
	// }

	// Log::logPairs(MEXC_tradingPairs);

	// while(true) {
	// 	time.start();
	// 	Log::log("Running main loop");

	// 	string prices = runCommand("node mexc-api/pair-prices.js");

	// 	std::istringstream iss(prices);

	// 	string pairName;
	// 	double price;

	// 	while (iss >> pairName >> price) {
	// 		MEXC_tradingPairs.pairs[pairName].updatePrice(price);

	// 		MEXC_tradingStrategy.trade(MEXC_tradingPairs.pairs[pairName]);
	// 	}

	// 	Log::LogWithTimestamp("Main update prices and MEXC_tradingStrategy.trades completed");

	// 	time.end();
	// 	Log::logPositions(positions);

	// 	double sleepTimeMins = 1;
	// 	double sleepTime = sleepTimeMins * 60 - time.getDuration();
	// 	if (sleepTime < 0) sleepTime = 0; 

	// 	cout << "Sleeping for " << sleepTime << " seconds" << endl;
	// 	Log::log("Sleeping for " + to_string(sleepTime) + " seconds");
	// 	time.sleep(sleepTime);			
	// }

	return 0;
}