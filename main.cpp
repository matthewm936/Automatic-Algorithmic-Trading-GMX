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

int main() {
	// setup logging file system
	Log::clearLogFiles();
	Log::LogWithTimestamp("MAIN.cpp Started");

	// setup objects
	Time time;

	unordered_map<string, Token> GMX_tokens;

	Positions positions;

	// import data from apis into objects
	runCommand("node gmx-api/gmx-rest-endpoints.js candles 10 1m 5m 15m 4h 1d");

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
			candlesticks.checkCandleOrderCorrectness();
			candlesticks.checkCandleMissingness();
			token.addCandlesticks(candlesticks);

			// cout << "Token: " << token.token << " Timeframe: " << candlesticks.getTimeFrame() << " Num candles: " << candlesticks.getCandles().size() << endl;

		}

		GMX_tokens[tokenIt.key()] = token;
	}


	// init done, starting main loop that runs every minute watching prices
	while(true) {
		time.start();

		runCommand("node gmx-api/gmx-rest-endpoints.js candles 2 1m");
		gmx_token_candles_data.clear();  // Clear the previous contents
		gmx_token_candles_data.seekg(0);  // Move the cursor back to the start of the file
		gmx_token_candles_data >> j;  // Read the new data
		
		for (nlohmann::json::iterator tokenIt = j.begin(); tokenIt != j.end(); ++tokenIt) { // does not seem to be updating the prices correvtly
			Token& token = GMX_tokens[tokenIt.key()];

			for (nlohmann::json::iterator timeframeIt = tokenIt->begin(); timeframeIt != tokenIt->end(); ++timeframeIt) {
				Candlesticks& candlesticks = token.getCandlesticks(timeframeIt.key());
				nlohmann::json candlesArray = timeframeIt->at("candles");

				for (nlohmann::json::iterator candleIt = candlesArray.begin(); candleIt != candlesArray.end(); ++candleIt) {
					Candle candle((*candleIt)[0].get<double>(), (*candleIt)[1].get<double>(), (*candleIt)[2].get<double>(), (*candleIt)[3].get<double>(), (*candleIt)[4].get<double>());

					candlesticks.addCandle(candle);
				}
				candlesticks.checkCandleOrderCorrectness();
				candlesticks.checkCandleMissingness();

				cout << "Token: " << token.token << " Timeframe: " << candlesticks.getTimeFrame() << " Num candles: " << candlesticks.getCandles().size() << endl;

				Log::logAndEmail(candlesticks.printStats());
			}
		}

		time.end();

		double sleepTimeMins = 1;
		double sleepTime = sleepTimeMins * 60 - time.getDuration();
		if (sleepTime < 0) sleepTime = 0; 

		time.sleep(sleepTime);			
	}

	return 0;
}