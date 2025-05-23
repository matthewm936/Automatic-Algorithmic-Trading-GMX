#include <deque>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <variant>
#include <iomanip> 
#include <ctime>
#include <set>

#include "Candle.cpp"
#include "Time.cpp"

using std::string;
using std::deque;
using std::cout;
using std::endl;
using std::to_string;

int MAX_NUM_CANDLES = 30;

const std::unordered_map<string, int> timeframeToSeconds = {
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
		string timeFrame = "";
		string token = "";
		deque<Candle>::size_type maxNumCandles = MAX_NUM_CANDLES;

		Time time;

		std::vector<int> missingTimestamps;

		int expectedMostRecentTimestamp(const string& timeFrame) {
			int offset = timeframeToSeconds.at(timeFrame);
			std::time_t currentTime = std::time(nullptr); // Get current Unix timestamp
			int mostRecentCandlestickTimestamp = currentTime - (currentTime % offset);
			return mostRecentCandlestickTimestamp;
		}

		int calculateTimestampDifference() {
			int expectedTimestamp = expectedMostRecentTimestamp(timeFrame);	
			return candles[0].timeStamp - expectedTimestamp;
		}

		void checkCandlesDescendingOrder() { 
			for (size_t i = 0; i < candles.size() - 1; i++) {
				if (candles[i].timeStamp < candles[i + 1].timeStamp) {
					Log::logError("Candles are not in the correct order, RUNTIME ERROR THROWN", true);
				}
			}
		}

		void checkForMissingCandles() { 
			int timeframeSeconds = timeframeToSeconds.at(timeFrame);
			for (size_t i = 1; i < candles.size(); ++i) {
				if(candles[i].timeStamp != candles[i - 1].timeStamp - timeframeSeconds) {
					string errorMessage = "ERROR; Timestamp errors\n";
					errorMessage += "Token: " + token + " Time frame: " + timeFrame + "\n";
					errorMessage += "Candles i-1 timestamp: " + to_string(candles[i - 1].timeStamp) + "\n";
					errorMessage += "Candles i   timestamp: " + to_string(candles[i].timeStamp) + "\n";
					Log::logError(errorMessage, true);
				}
			}
		}

		void checkCandleUpToDate() {
			int offset = timeframeToSeconds.at(timeFrame);
			int expectedTimestamp = expectedMostRecentTimestamp(timeFrame);
			if(candles[0].timeStamp <= expectedTimestamp - (2 * offset)){
				Log::logError("ERROR; most recent candle behind by more then two candles", true);
				throw std::runtime_error("ERROR; most recent candle behind by more then two candles");
			}	
		}

	public:
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
		double higherLowsPercent;

		double lowerHighsPercent;
		double lowerLowsPercent;

		int	numHigherOpens;
		int numHigherCloses;
		int	numLowerOpens;
		int	numLowerCloses;

		double higherOpensPercent;
		double higherClosesPercent;

		double lowerOpensPercent;
		double lowerClosesPercent;

		double average;
		double averageReturn;
		double averageLogReturn;
		double averageLogHighLowReturn;

		Candlesticks() {};
		
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

		void checkCandleCorrectness() {
			checkCandlesDescendingOrder();
			checkForMissingCandles();
			checkCandleUpToDate();
		}

		void calculateCandleStatistics() { 
			calculateCandleStatistics(0, candles.size());
		}

		void calculateCandleStatistics(int start, int end) { 
			if(end > MAX_NUM_CANDLES) { Log::logError("attempting to calculate candle stats for more than MAX_NUM_CANDLES candles"); }

			int candlesChecked = end - start + 1;
			int candlePairsChecked = candlesChecked - 1;

			double averageSum = 0;
			double averageReturnSum = 0;
			double logReturnSum = 0;
			double logHighLowReturnSum = 0;

			numGreenCandles = 0;
			numRedCandles = 0;
			numDojiCandles = 0;

			for(int i = start; i <= end; i++) {
				const auto& candle = candles[i];
				if(candle.green) numGreenCandles++;
				if(candle.red) numRedCandles++;
				if(candle.doji) numDojiCandles++;

				averageSum += candle.Average;
				averageReturnSum += candle.PercentChange;
				logReturnSum += candle.LogPercentChange;
				logHighLowReturnSum += candle.LogHighLowPercentChange;
			}

			average = averageSum / (double)candlesChecked;
			averageReturn = averageReturnSum / (double)candlesChecked;
			averageLogReturn = logReturnSum / (double)candlesChecked;
			averageLogHighLowReturn = logHighLowReturnSum/ (double) candlesChecked;

			greenRedRatio = (double)numGreenCandles / (double)numRedCandles;
			redGreenRatio = (double)numRedCandles / (double)numGreenCandles;
			greenCandlePercent = (double)numGreenCandles / (double)candlesChecked;
			redCandlePercent = (double)numRedCandles / (double)candlesChecked;

			numHigherHighs = 0;
			numLowerLows = 0;
			numHigherLows = 0;
			numLowerHighs = 0;
			
			numHigherOpens = 0;
			numHigherCloses = 0;
			numLowerOpens = 0;
			numLowerCloses = 0;

			for(int i = start; i <= end - 1; i++) {
				if(candles[i].high > candles[i + 1].high) numHigherHighs++;
				if(candles[i].low < candles[i + 1].low) numLowerLows++;
				if(candles[i].low > candles[i + 1].low) numHigherLows++;
				if(candles[i].high < candles[i + 1].high) numLowerHighs++;

				if(candles[i].open > candles[i + 1].open) numHigherOpens++;
				if(candles[i].close > candles[i + 1].close) numHigherCloses++;
				if(candles[i].open < candles[i + 1].open) numLowerOpens++;
				if(candles[i].close < candles[i + 1].close) numLowerCloses++;
			}

			higherHighsPercent = (double)numHigherHighs / (double)candlePairsChecked;
			lowerLowsPercent = (double)numLowerLows / (double)candlePairsChecked;
			higherLowsPercent = (double)numHigherLows / (double)candlePairsChecked;
			lowerHighsPercent = (double)numLowerHighs / (double)candlePairsChecked;

			higherOpensPercent = (double)numHigherOpens / (double)candlePairsChecked;
			higherClosesPercent = (double)numHigherCloses / (double)candlePairsChecked;	
			lowerOpensPercent = (double)numLowerOpens / (double)candlePairsChecked;
			lowerClosesPercent = (double)numLowerCloses / (double)candlePairsChecked;
		}

		Candle getHighestCandle(int start, int end) {
			if(end > MAX_NUM_CANDLES) { Log::logError("attempting to calculate candle stats for more than MAX_NUM_CANDLES candles"); }
			Candle highestCandle = candles[start];
			for(int i = start; i < end; i++) {
				const auto& candle = candles[i];
				if(candle.high > highestCandle.high) highestCandle = candle;
			}		
			return highestCandle;
		}

		Candle getLowestCandle(int start, int end) {
			if(end > MAX_NUM_CANDLES) { Log::logError("attempting to calculate candle stats for more than MAX_NUM_CANDLES candles"); }
			Candle lowestCandle = candles[start];
			for(int i = start; i < end; i++) {
				const auto& candle = candles[i];
				if(candle.low < lowestCandle.low) lowestCandle = candle;
			}		
			return lowestCandle;
		}

		string getTimeFrame() const {
			return timeFrame;
		}

		string getTokenName() const {
			return token;
		}

		double getCurrentPrice() const {
			return candles[0].close;
		}

		deque<Candle>& getCandles() {
			return candles;
		}

};