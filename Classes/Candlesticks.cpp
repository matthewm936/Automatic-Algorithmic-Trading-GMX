#include "Candle.cpp"

#include <deque>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <variant>
#include <iomanip> 
#include <ctime>

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

		std::vector<int> missingTimestamps;

		int calculateMostRecentCandlestickTimestamp(const std::string& timeFrame) {
			int offset = timeFrameToUnixOffset.at(timeFrame);
			std::time_t currentTime = std::time(nullptr); // Get current Unix timestamp
			int mostRecentCandlestickTimestamp = currentTime - (currentTime % offset);
			return mostRecentCandlestickTimestamp;
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

		int	numIncreasingOpens;
		int numIncreasingCloses;
		int	numDecreasingOpens;
		int	numDecreasingCloses;

		double increasingOpensPercent;
		double increasingClosesPercent;
		double decreasingOpensPercent;
		double decreasingClosesPercent;

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
		}


		void checkCandlesDescendingOrder() { 
			for (size_t i = 0; i < candles.size() - 1; i++) {
				if (candles[i].timeStamp > candles[i + 1].timeStamp) {
					// correct order, most recent at the front
				} else {
					Log::logError("Candles are not in the correct order, RUNTIME ERROR THROWN", true);
				}
			}
		}

		void checkCandleMissingness() { 
			int offset = timeFrameToUnixOffset.at(timeFrame);
			int expectedTimestamp = calculateMostRecentCandlestickTimestamp(timeFrame);
			if(candles[0].timeStamp == expectedTimestamp - offset) {
				std::string errorMessage = token + " " + timeFrame + " Candle Behind by " + to_string(offset) + "\n";
				errorMessage += "Expected timestamp: " + to_string(expectedTimestamp) + " Got candle timestamp: " + to_string(candles[0].timeStamp) + " difference:" + to_string(candles[0].timeStamp - expectedTimestamp);
				
				Log::logError(errorMessage);
				expectedTimestamp = expectedTimestamp - offset;
			} 

			for (size_t i = 0; i < candles.size(); ++i) {
				if(candles[i].timeStamp != expectedTimestamp) {
					if (std::find(missingTimestamps.begin(), missingTimestamps.end(), expectedTimestamp) == missingTimestamps.end()) {
						std::string errorMessage = "Candles Timestamp errors, possible missing candle\n";
						// errorMessage += "Token: " + token + " Time frame: " + timeFrame + "\n";
						// errorMessage += "Expected timestamp: " + to_string(expectedTimestamp) + "\n";
						// errorMessage += "Got candle timestamp: " + to_string(candles[i].timeStamp) + "\n";

						Log::logError(errorMessage);
						missingTimestamps.push_back(expectedTimestamp);
					}
				}
				expectedTimestamp -= offset;
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
			
			numIncreasingOpens = 0;
			numIncreasingCloses = 0;
			numDecreasingOpens = 0;
			numDecreasingCloses = 0;

			for(int i = start; i < end - 1; i++) {
				if(candles[i].high > candles[i + 1].high) numHigherHighs++;
				if(candles[i].low < candles[i + 1].low) numLowerLows++;
				if(candles[i].low > candles[i + 1].low) numHigherLows++;
				if(candles[i].high < candles[i + 1].high) numLowerHighs++;

				if(candles[i].open > candles[i + 1].open) numIncreasingOpens++;
				if(candles[i].close > candles[i + 1].close) numIncreasingCloses++;
				if(candles[i].open < candles[i + 1].open) numDecreasingOpens++;
				if(candles[i].close < candles[i + 1].close) numDecreasingCloses++;
			}

			higherHighsPercent = (double)numHigherHighs / (double)candlePairsChecked;
			lowerLowsPercent = (double)numLowerLows / (double)candlePairsChecked;
			higherLowsPercent = (double)numHigherLows / (double)candlePairsChecked;
			lowerHighsPercent = (double)numLowerHighs / (double)candlePairsChecked;

			increasingOpensPercent = (double)numIncreasingOpens / (double)candlePairsChecked;
			increasingClosesPercent = (double)numIncreasingCloses / (double)candlePairsChecked;	
			decreasingOpensPercent = (double)numDecreasingOpens / (double)candlePairsChecked;
			decreasingClosesPercent = (double)numDecreasingCloses / (double)candlePairsChecked;
		}

		string getTimeFrame() {
			return timeFrame;
		}

		deque<Candle>& getCandles() {
			return candles;
		}

		string getStats() {
			calculateCandleStatistics();
			std::ostringstream stats;
			stats << std::fixed << std::setprecision(2);  
			stats << "\n";
			stats << "Token: " << token << "\n";
			stats << "Current price: " << currentPrice << "\n";
			stats << "Time frame: " << timeFrame << "\n";
			stats << "Num candles: " << candles.size() << "\n";
			stats << "Highest candle: " << highestCandle.high << "\n";
			stats << "Lowest candle: " << lowestCandle.low << "\n";
			stats << "Green candles: " << numGreenCandles << "\n";
			stats << "Red candles: " << numRedCandles << "\n";
			stats << "Doji candles: " << numDojiCandles << "\n";
			stats << "Green red ratio: " << greenRedRatio << "\n";
			stats << "Red green ratio: " << redGreenRatio << "\n";
			stats << "Green candle percent: " << greenCandlePercent << "\n";
			stats << "Red candle percent: " << redCandlePercent << "\n";
			stats << "Num higher highs: " << numHigherHighs << "\n";
			stats << "Num lower lows: " << numLowerLows << "\n";
			stats << "Num higher lows: " << numHigherLows << "\n";
			stats << "Num lower highs: " << numLowerHighs << "\n";
			stats << "Higher highs percent: " << higherHighsPercent << "\n";
			stats << "Lower lows percent: " << lowerLowsPercent << "\n";
			stats << "Higher lows percent: " << higherLowsPercent << "\n";
			stats << "Lower highs percent: " << lowerHighsPercent << "\n";
			stats << "Increasing opens percent: " << increasingOpensPercent << "\n";
			stats << "Increasing closes percent: " << increasingClosesPercent << "\n";
			stats << "Decreasing opens percent: " << decreasingOpensPercent << "\n";
			stats << "Decreasing closes percent: " << decreasingClosesPercent << "\n";

			return stats.str();
		}
};