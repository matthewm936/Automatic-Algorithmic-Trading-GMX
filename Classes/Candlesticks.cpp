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

int MAX_NUM_CANDLES = 30;

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
			string test = "test";
			Log::logError(test);
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

		void calculateCandleStatistics() {
			calculateCandleStatistics(0, candles.size());
		}

		void calculateCandleStatistics(int start, int end) {
			if(end > MAX_NUM_CANDLES) { Log::logError("attempting to calculate candle stats for more than MAX_NUM_CANDLES candles"); }

			int candlesChecked = end - start;
			int candlePairsChecked = candlesChecked - 1;

			highestCandle = candles[start];
			lowestCandle = candles[start];
			numGreenCandles = 0;
			numRedCandles = 0;
			numDojiCandles = 0;

			for(int i = start; i < end; i++) {
				const auto& candle = candles[i];
				if(candle.high > highestCandle.high) highestCandle = candle;
				if(candle.low < lowestCandle.low) lowestCandle = candle;
				if(candle.green) numGreenCandles++;
				if(candle.red) numRedCandles++;
				if(candle.doji) numDojiCandles++;
			}

			greenRedRatio = (double)numGreenCandles / (double)numRedCandles;
			redGreenRatio = (double)numRedCandles / (double)numGreenCandles;
			greenCandlePercent = (double)numGreenCandles / (double)candlesChecked;
			redCandlePercent = (double)numRedCandles / (double)candlesChecked;
			currentPrice = candles[0].close;

			numHigherHighs = 0;
			numLowerLows = 0;
			numHigherLows = 0;
			numLowerHighs = 0;

			for(int i = start; i < end - 1; i++) {
				if(candles[i].high > candles[i + 1].high) numHigherHighs++;
				if(candles[i].low < candles[i + 1].low) numLowerLows++;
				if(candles[i].low > candles[i + 1].low) numHigherLows++;
				if(candles[i].high < candles[i + 1].high) numLowerHighs++;
			}

			higherHighsPercent = (double)numHigherHighs / (double)candlePairsChecked;
			lowerLowsPercent = (double)numLowerLows / (double)candlePairsChecked;
			higherLowsPercent = (double)numHigherLows / (double)candlePairsChecked;
			lowerHighsPercent = (double)numLowerHighs / (double)candlePairsChecked;
		}

	public:
		Candle highestCandle;
		Candle lowestCandle;

		int numGreenCandles;
		int numRedCandles;
		int numDojiCandles;

		double greenRedRatio;
		double redGreenRatio;

		double greenCandlePercent;
		double redCandlePercent;

		int numHigherHighs;
		int numLowerLows;
		int numHigherLows;
		int numLowerHighs;

		double higherHighsPercent;
		double lowerLowsPercent;
		double higherLowsPercent;
		double lowerHighsPercent;

		double currentPrice;

		Candlesticks() {
		};
		
		Candlesticks(string timeFrame, string token) {
			this->timeFrame = timeFrame;
			this->token = token;
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

			calculateCandleStatistics(); //update stats on each candle addition

			checkCandleMissingness();
			checkCandlesDescendingOrder();
		}

		string getTimeFrame() {
			return timeFrame;
		}

		deque<Candle>& getCandles() {
			return candles;
		}

		string getStats() {
			calculateCandleStatistics();
			string stats = "\n";
			stats += "Time frame: " + timeFrame + "\n";
			stats += "Num candles: " + to_string(candles.size()) + "\n";
			stats += "Highest candle: " + to_string(highestCandle.high) + "\n";
			stats += "Lowest candle: " + to_string(lowestCandle.low) + "\n";
			stats += "Green candles: " + to_string(numGreenCandles) + "\n";
			stats += "Red candles: " + to_string(numRedCandles) + "\n";
			stats += "Doji candles: " + to_string(numDojiCandles) + "\n";
			stats += "Green red ratio: " + to_string(greenRedRatio) + "\n";
			stats += "Red green ratio: " + to_string(redGreenRatio) + "\n";
			stats += "Green candle percent: " + to_string(greenCandlePercent) + "\n";
			stats += "Red candle percent: " + to_string(redCandlePercent) + "\n";
			stats += "Num higher highs: " + to_string(numHigherHighs) + "\n";
			stats += "Num lower lows: " + to_string(numLowerLows) + "\n";
			stats += "Num higher lows: " + to_string(numHigherLows) + "\n";
			stats += "Num lower highs: " + to_string(numLowerHighs) + "\n";
			stats += "Higher highs percent: " + to_string(higherHighsPercent) + "\n";
			stats += "Lower lows percent: " + to_string(lowerLowsPercent) + "\n";
			stats += "Higher lows percent: " + to_string(higherLowsPercent) + "\n";
			stats += "Lower highs percent: " + to_string(lowerHighsPercent) + "\n";
			stats += "Current price: " + to_string(currentPrice) + "\n";
			return stats;
		}
};