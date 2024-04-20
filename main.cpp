#include <cstdlib>
#include <thread>
#include <chrono>
#include <cstdio>
#include <map>
#include <locale>
#include <sstream>

#include "nlohmann/json.hpp"

#include "Classes/RunCommand.cpp"

#include "Classes/Token.cpp"

#include "Classes/Positions.cpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

using json = nlohmann::json;

string formatWithCommas(int value) {
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

Candle createCandle(const nlohmann::json& jsonArray) {
	return Candle(jsonArray[0].get<double>(), jsonArray[1].get<double>(), jsonArray[2].get<double>(), jsonArray[3].get<double>(), jsonArray[4].get<double>());
} 

int main() {
	Log::clearLogFiles();
	Log::LogWithTimestamp("MAIN.cpp Started");

	Time time;
	unordered_map<string, Token> GMX_tokens;
	Positions positions;

	runCommand("node gmx-api/gmx-rest-endpoints.js candles 30 1m 5m 15m 1h 4h 1d");

	std::ifstream gmx_token_candles_data("gmx-api/token-candles.json");
	nlohmann::json j;
	gmx_token_candles_data >> j;

	for (nlohmann::json::iterator tokenIt = j.begin(); tokenIt != j.end(); ++tokenIt) {
		Token token(tokenIt.key());

		for (nlohmann::json::iterator timeframeIt = tokenIt->begin(); timeframeIt != tokenIt->end(); ++timeframeIt) {
			Candlesticks candlesticks(timeframeIt.key(), token.token);
			nlohmann::json candlesArray = timeframeIt->at("candles");

			for (nlohmann::json::iterator candleIt = candlesArray.begin(); candleIt != candlesArray.end(); ++candleIt) {
				candlesticks.addCandle(createCandle(*candleIt));
			}
			token.addCandlesticks(candlesticks);
		}
		GMX_tokens[tokenIt.key()] = token;
	}

	string updateCandleStickDataCommand = "node gmx-api/gmx-rest-endpoints.js candles 4 1m 5m 15m 1h 4h 1d";

	while(true) {
		time.start();

		runCommand(updateCandleStickDataCommand.c_str()); 

		gmx_token_candles_data.clear(); 
		gmx_token_candles_data.seekg(0); 

		try {
			gmx_token_candles_data >> j;  
		} catch (json::parse_error& e) {
			Log::logError("JSON parse error: " + string(e.what()), true);
			std::cerr << "JSON parse error: " << e.what() << '\n';
		}		

		try {
			for (nlohmann::json::iterator tokenIt = j.begin(); tokenIt != j.end(); ++tokenIt) {
				Token& token = GMX_tokens[tokenIt.key()];

				for (nlohmann::json::iterator timeframeIt = tokenIt->begin(); timeframeIt != tokenIt->end(); ++timeframeIt) {
					Candlesticks& candlesticks = token.getCandlesticks(timeframeIt.key());
					nlohmann::json candlesArray = timeframeIt->at("candles");

					for (nlohmann::json::iterator candleIt = candlesArray.begin(); candleIt != candlesArray.end(); ++candleIt) {
						candlesticks.addCandle(createCandle(*candleIt));
					}
					candlesticks.checkForMissingCandles();
					candlesticks.checkCandleUpToDate();
					candlesticks.checkCandlesDescendingOrder();

					candlesticks.calculateCandleStatistics(0, 4);

					if(candlesticks.higherHighsPercent == 1 && candlesticks.higherLowsPercent == 1) {
						Log::logAndEmail("BUY SIGNAL: " + token.token + " " + timeframeIt.key());
					} else if(candlesticks.lowerHighsPercent == 1 && candlesticks.lowerLowsPercent == 1) {
						Log::logAndEmail("SELL SIGNAL: " + token.token + " " + timeframeIt.key());
					}
				}
			}
		} catch (const std::exception& e) {
			Log::logError("Error: " + string(e.what()), true);
			std::cerr << "Error: " << e.what() << '\n';
		}

		time.end();
		Log::log(time.sleep());
	}

	return 0;
}