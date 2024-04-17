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

	runCommand("node gmx-api/gmx-rest-endpoints.js candles 20 1m 5m 15m 1h 4h 1d");

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

	string updateCandleStickDataCommand = "node gmx-api/gmx-rest-endpoints.js candles 2 1m 5m 15m 1h 4h 1d"; // update current price i.e the close of the current canlde every 30s

	while(true) {
		time.start();

		runCommand(updateCandleStickDataCommand.c_str()); 

		gmx_token_candles_data.clear();  // Clear the previous contents
		gmx_token_candles_data.seekg(0);  // Move the cursor back to the start of the file

		try {
			gmx_token_candles_data >> j;  // Read the new data
		} catch (json::parse_error& e) {
			Log::logError("JSON parse error: " + string(e.what()));
			std::cerr << "JSON parse error: " << e.what() << '\n';
		}		

		try {
			for (nlohmann::json::iterator tokenIt = j.begin(); tokenIt != j.end(); ++tokenIt) { // if a token is grabbed that the init didnt get or a candlestick timeframe, this will cause unordermap errors which exit the program, so ig just try catch for now, this might need to be
				Token& token = GMX_tokens[tokenIt.key()];

				for (nlohmann::json::iterator timeframeIt = tokenIt->begin(); timeframeIt != tokenIt->end(); ++timeframeIt) {
					Candlesticks& candlesticks = token.getCandlesticks(timeframeIt.key());
					nlohmann::json candlesArray = timeframeIt->at("candles");

					for (nlohmann::json::iterator candleIt = candlesArray.begin(); candleIt != candlesArray.end(); ++candleIt) {
						candlesticks.addCandle(createCandle(*candleIt));
					}

					string movement = candlesticks.movement();
					string email = "Token " + token.token + " Timeframe " + candlesticks.getTimeFrame() + " is " + movement + " trending of ";
					if(movement == "green") {
						email += to_string(candlesticks.greenCandlePercent * 100) + "%" + " at price " + to_string(candlesticks.getCandles()[0].close);
						Log::logAndEmail(email.c_str());
					}
					if(movement == "red") {
						email += to_string(candlesticks.redCandlePercent * 100) + "%" + " at price " + to_string(candlesticks.getCandles()[0].close);
						Log::logAndEmail(email.c_str());
					}
				}
			}
		} catch (const std::exception& e) {
			Log::logError("Error: " + string(e.what()));
			std::cerr << "Error: " << e.what() << '\n';
		}

		time.end();
		Log::log(time.sleep()); //sleeps then logs when the sleep occured, not sure if this is what I want
	}

	return 0;
}