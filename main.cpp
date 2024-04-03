#include <cstdlib>
#include <thread>
#include <chrono>
#include <cstdio>
#include <map>
#include <locale>
#include <sstream>

#include "Classes/Headers/RunCommand.h"

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

	// import data from apis into objects
	runCommand("node gmx-api/gmx-rest-endpoints.js candles");

	std::ifstream gmx_token_candles_data("gmx-api/token-candles.json");
	nlohmann::json j;
	gmx_token_candles_data >> j;

	for (nlohmann::json::iterator tokenIt = j.begin(); tokenIt != j.end(); ++tokenIt) {
		Token token(tokenIt.key());

		for (nlohmann::json::iterator timeframeIt = tokenIt->begin(); timeframeIt != tokenIt->end(); ++timeframeIt) {
			Candlesticks candlesticks(timeframeIt.key());
			nlohmann::json candlesArray = timeframeIt->at("candles");

			for (nlohmann::json::iterator candleIt = candlesArray.begin(); candleIt != candlesArray.end(); ++candleIt) {
				Candle candle((*candleIt)[0].get<double>(), (*candleIt)[1].get<double>(), (*candleIt)[2].get<double>(), (*candleIt)[3].get<double>(), (*candleIt)[4].get<double>());

				candlesticks.addCandle(candle);
			}
			token.addCandlesticks(candlesticks);
		}
	}

	runCommand("node mexc-api/ticker24hrALL.js");

	std::ifstream i("prices.json");
	i >> j;

	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		string symbol = it->at("symbol").get<std::string>();
		double lastPrice = getDoubleFromJson(it, "lastPrice");
		double askPrice = getDoubleFromJson(it, "askPrice");
		double bidPrice = getDoubleFromJson(it, "bidPrice");
		double askQty = getDoubleFromJson(it, "askQty");
		double bidQty = getDoubleFromJson(it, "bidQty");
		double volume = getDoubleFromJson(it, "volume");
		double quoteVolume = getDoubleFromJson(it, "quoteVolume");

		MEXC_tradingPairs.addPair(lastPrice, symbol, askPrice, bidPrice, askQty, bidQty, volume, quoteVolume);
	}

	// init done, starting main loop that runs every minute watching prices
	while(true) {
		time.start();

		string prices = runCommand("node mexc-api/pair-prices.js");

		std::istringstream iss(prices);

		string pairName;
		double price;

		while (iss >> pairName >> price) {
			MEXC_tradingPairs.pairs[pairName].updatePrice(price);

			MEXC_tradingStrategy.trade(MEXC_tradingPairs.pairs[pairName]);
		}

		time.end();
		Log::logPositions(positions);

		double sleepTimeMins = 1;
		double sleepTime = sleepTimeMins * 60 - time.getDuration();
		if (sleepTime < 0) sleepTime = 0; 

		time.sleep(sleepTime);			
	}

	return 0;
}