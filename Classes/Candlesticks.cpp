#include "Candle.cpp"

#include <deque>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <variant>

using std::string;
using std::deque;
using std::cout;
using std::endl;
using std::to_string;

int MAX_NUM_CANDLES = 20;

const std::unordered_map<string, int> timeFrameToUnixOffset = {
	{"1m", 60},
	{"5m", 300},
	{"15m", 900},
	{"1h", 3600},
	{"4h", 14400},
	{"1d", 86400}
};

class Candlesticks {
	private:
		std::deque<Candle> candles; // DESCENDING order; ie most recent at the front
		std::string timeFrame = "";
		std::string token = "";
		deque<Candle>::size_type maxNumCandles = MAX_NUM_CANDLES;

	public:
		Candlesticks() {
			// std::cout << "Candlesticks object created" << std::endl;
		};
		
		Candlesticks(string timeFrame, string token) {
			this->timeFrame = timeFrame;
			this->token = token;

			// std::cout << "Candlesticks object created with time frame: " << timeFrame << std::endl;
		}

		int getSize() {
			return candles.size();
		}

		void addCandle(Candle candle) { 
			for(auto& c : candles) {
				if(c.timeStamp == candle.timeStamp) { //update candle, bc non-closed candles are still sent as a candle in progress so anytime you get the new a candle with the same timestamp update it
					//todo test this new adition which is updating current in progress candles
					c = candle;
					return;
				}
			}
			auto pos = std::find_if(candles.begin(), candles.end(), [&candle](const Candle& c) {
				return c.timeStamp < candle.timeStamp;
			});
			candles.insert(pos, candle);
		
			if(candles.size() > maxNumCandles) {
				candles.pop_back();
			}
		}

		string getTimeFrame() {
			return timeFrame;
		}

		deque<Candle>& getCandles() {
			return candles;
		}

		void checkCandlesDescendingOrder() {
			for (size_t i = 0; i < candles.size() - 1; i++) {
				if (candles[i].timeStamp > candles[i + 1].timeStamp) {
					// correct order, most recent at the front
				} else {
					Log::logError("Candles are not in the correct order, RUNTIME ERROR THROWN");
					throw std::runtime_error("Candles are not in the correct order");
				}
			}
		}

		void checkCandleMissingness() {
			int offset = timeFrameToUnixOffset.at(timeFrame);
			for (size_t i = 1; i < candles.size(); ++i) {
				if (candles[i-1].timeStamp - offset != candles[i].timeStamp) {
					std::string errorMessage = "CANDLE MISSING\n";
					errorMessage += "Token: " + token + " Time frame: " + timeFrame + "\n";
					errorMessage += "Expected offset: " + to_string(offset) + "\n";
					errorMessage += "Difference: " + to_string(candles[i].timeStamp - candles[i-1].timeStamp) + " at index i: " + to_string(i) + "\n";
					errorMessage += "Candle i-1 timestamp: " + to_string(candles[i-1].timeStamp) + "\n";
					errorMessage += "Candle i timestamp: " + to_string(candles[i].timeStamp) + "\n";

					Log::logError(errorMessage);
					throw std::runtime_error("Candles are missing");
				}
			}
		}

		std::map<string, std::variant<Candle, int, double>> getCandleInfo() { //untested todo
			return getCandleInfo(0, candles.size());
		}

		std::map<string, std::variant<Candle, int, double>> getCandleInfo(int start, int end) { //untested
			Candle highestCandle = candles[start];
			Candle lowestCandle = candles[start];
			int greenCandles = 0;
			int redCandles = 0;
			int dojiCandles = 0;

			for(int i = start; i < end; i++) {
				const auto& candle = candles[i];
				if(candle.high > highestCandle.high) highestCandle = candle;
				if(candle.low < lowestCandle.low) lowestCandle = candle;
				if(candle.green) greenCandles++;
				if(candle.red) redCandles++;
				if(candle.doji) dojiCandles++;
			}

			std::map<std::string, std::variant<Candle, int, double>> candleInfo;
			candleInfo["highestCandle"] = highestCandle;
			candleInfo["lowestCandle"] = lowestCandle;
			candleInfo["numGreenCandles"] = greenCandles;
			candleInfo["numRedCandles"] = redCandles;
			candleInfo["numDojiCandles"] = dojiCandles;
			candleInfo["greenRedRatio"] = (double)greenCandles / (double)redCandles;
			candleInfo["greenCandlePercent"] = (double)greenCandles / (double)(end - start);
			candleInfo["redCandlePercent"] = (double)redCandles / (double)(end - start);

			return candleInfo;
		}

		double getCloseAbovePrevClosePercent() {
			int closeAbovePrevClose = 0;
			for(size_t i = candles.size() - 1; i > 0; --i) { // is this loop condition correct
				if(candles[i].close > candles[i-1].close) { //todo this if might need to be <
					closeAbovePrevClose += 1;
				} 
			} 
			return (double)closeAbovePrevClose / (double)candles.size();
		}

		string movement() {
			auto candleInfo = getCandleInfo();
			double greenCandlePercent = std::get<double>(candleInfo["greenCandlePercent"]);
			double redCandlePercent = std::get<double>(candleInfo["redCandlePercent"]);

			if(greenCandlePercent <= 0.65 && 0.35 <= greenCandlePercent) {
				return "ranging";
			}
			if(redCandlePercent <= 0.65 && 0.45 <= redCandlePercent) {
				return "ranging";
			}
			if(greenCandlePercent > 0.85) {
				return "green";
			}
			if(redCandlePercent > 0.85) {
				return "red";
			} else {
				return "unknown";
			}
		}

		string getStats() {
			auto candleInfo = getCandleInfo();
			string stats = "\n";
			stats += "Time frame: " + timeFrame + "\n";
			stats += "Num candles: " + to_string(candles.size()) + "\n";
			stats += "Highest candle: " + to_string(std::get<Candle>(candleInfo["highestCandle"]).high) + "\n";
			stats += "Lowest candle: " + to_string(std::get<Candle>(candleInfo["lowestCandle"]).low) + "\n";
			stats += "Green candles: " + to_string(std::get<int>(candleInfo["numGreenCandles"])) + "\n";
			stats += "Red candles: " + to_string(std::get<int>(candleInfo["numRedCandles"])) + "\n";
			stats += "Doji candles: " + to_string(std::get<int>(candleInfo["numDojiCandles"])) + "\n";
			stats += "Green red ratio: " + to_string(std::get<double>(candleInfo["greenRedRatio"])) + "\n";
			stats += "Green candle percent: " + to_string(std::get<double>(candleInfo["greenCandlePercent"])) + "\n";
			stats += "Red candle percent: " + to_string(std::get<double>(candleInfo["redCandlePercent"])) + "\n";
			stats += "Close above prev close percent: " + to_string(getCloseAbovePrevClosePercent()) + "\n";
			return stats;
		}
};