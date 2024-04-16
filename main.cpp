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

	runCommand("node gmx-api/gmx-rest-endpoints.js candles 10 1m 5m 15m 1h 4h 1d");

	std::ifstream gmx_token_candles_data("gmx-api/token-candles.json");
	nlohmann::json j;
	gmx_token_candles_data >> j;

	for (nlohmann::json::iterator tokenIt = j.begin(); tokenIt != j.end(); ++tokenIt) {
		Token token(tokenIt.key());

		for (nlohmann::json::iterator timeframeIt = tokenIt->begin(); timeframeIt != tokenIt->end(); ++timeframeIt) {
			Candlesticks candlesticks(timeframeIt.key());
			nlohmann::json candlesArray = timeframeIt->at("candles");

			for (nlohmann::json::iterator candleIt = candlesArray.begin(); candleIt != candlesArray.end(); ++candleIt) {
				candlesticks.addCandle(createCandle(*candleIt));
			}
			candlesticks.checkCandleOrderCorrectness();
			candlesticks.checkCandleMissingness();
			token.addCandlesticks(candlesticks);
		}
		GMX_tokens[tokenIt.key()] = token;
	}

	string baseCommand = "node gmx-api/gmx-rest-endpoints.js candles 2";
	string updateCandleStickDataCommand = baseCommand + " 1m";

	while(true) {
		time.start();

		int unixTime = std::stoi(time.getUnixTime());

		int timeMod5min = unixTime % 300;
		int timeMod15min = unixTime % 900;
		int timeMod1h = unixTime % 3600;
		int timeMod4h = unixTime % 14400;
		int timeMod1d = unixTime % 86400;

		if(timeMod5min >= 0 && timeMod5min < 60) {
			updateCandleStickDataCommand += " 5m";
		}
		if(timeMod15min >= 0 && timeMod15min < 60) {
			updateCandleStickDataCommand += " 15m";
		}
		if(timeMod1h >= 0 && timeMod1h < 60) {
			updateCandleStickDataCommand += " 1h";
		}
		if(timeMod4h >= 0 && timeMod4h < 60) {
			updateCandleStickDataCommand += " 4h";
		}
		if(timeMod1d >= 0 && timeMod1d < 60) {
			updateCandleStickDataCommand += " 1d";
		}

		runCommand(updateCandleStickDataCommand.c_str()); 

		updateCandleStickDataCommand = baseCommand + " 1m";

		gmx_token_candles_data.clear();  // Clear the previous contents
		gmx_token_candles_data.seekg(0);  // Move the cursor back to the start of the file

		try {
			gmx_token_candles_data >> j;  // Read the new data
		} catch (json::parse_error& e) {
			Log::logError("JSON parse error: " + string(e.what()));
			std::cerr << "JSON parse error: " << e.what() << '\n';
		}		

		try {
			for (nlohmann::json::iterator tokenIt = j.begin(); tokenIt != j.end(); ++tokenIt) { // if a token is grabbed that the init didnt get or a candlestick timeframe, this will cause unordermap errors which exit the program, so ig just try catch for now, this might need to be revisited
				Token& token = GMX_tokens[tokenIt.key()];

				for (nlohmann::json::iterator timeframeIt = tokenIt->begin(); timeframeIt != tokenIt->end(); ++timeframeIt) {
					Candlesticks& candlesticks = token.getCandlesticks(timeframeIt.key());
					nlohmann::json candlesArray = timeframeIt->at("candles");

					for (nlohmann::json::iterator candleIt = candlesArray.begin(); candleIt != candlesArray.end(); ++candleIt) {
						candlesticks.addCandle(createCandle(*candleIt));
					}
					candlesticks.checkCandleOrderCorrectness();
					candlesticks.checkCandleMissingness();

					if(candlesticks.getTimeFrame() != "1m" && candlesticks.getTimeFrame() != "5m") {
						Log::LogWithTimestamp("Token: " + token.token + " Timeframe: " + candlesticks.getTimeFrame() + " data updated" + candlesticks.getStats());

						string movement = candlesticks.movement();
						string email = "Token " + token.token + " Timeframe " + candlesticks.getTimeFrame() + " is " + movement + " trending of ";
						if(movement == "green") {
							email += std::to_string(candlesticks.getGreenCandlePercent()) + "%" + " at price " + std::to_string(candlesticks.getCandles()[0].close);
							Log::logAndEmail(email.c_str());
						}
						if(movement == "red") {
							email += std::to_string(candlesticks.getRedCandlePercent()) + "%" + " at price " + std::to_string(candlesticks.getCandles()[0].close);
							Log::logAndEmail(email.c_str());
						}
					}
				}
			}
		} catch {
			Log::logError("Error: " + string(e.what()));
			std::cerr << "Error: " << e.what() << '\n';
		}


		time.end();
		Log::log(time.sleep()); //sleeps then logs when the sleep occured, not sure if this is what I want
	}

	return 0;
}