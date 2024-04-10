#include "Candle.cpp"

#include <deque>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using std::string;
using std::deque;
using std::cout;
using std::endl;


int MAX_NUM_CANDLES = 50;

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
		std::deque<Candle> candles;
		std::string timeFrame = "";
		deque<Candle>::size_type maxNumCandles = MAX_NUM_CANDLES;

	public:
		Candlesticks() {
			// std::cout << "Candlesticks object created" << std::endl;
		};
		
		Candlesticks(string timeFrame) {
			this->timeFrame = timeFrame;

			// std::cout << "Candlesticks object created with time frame: " << timeFrame << std::endl;
		}

		

		int getSize() {
			return candles.size();
		}

		void addCandle(Candle candle) { 
			for(const auto& c : candles) {
				if(c.timeStamp == candle.timeStamp) {
					std::cout << "Candle with same timestamp already exists" << std::endl;
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

			cout << "Candle added to candlesticks" << endl;
			cout << "timeFrame: " << timeFrame << endl;
		}

		string getTimeFrame() {
			return timeFrame;
		}

		deque<Candle>& getCandles() {
			return candles;
		}

		void checkCandleOrderCorrectness() {
			for (size_t i = 1; i < candles.size(); ++i) {
				if (candles[i-1].timeStamp < candles[i].timeStamp) {
					throw std::runtime_error("Candles are not in the correct order");
				}
			}
		}

		void checkCandleMissingness() {
			int offset = timeFrameToUnixOffset.at(timeFrame);
			for (size_t i = 1; i < candles.size(); ++i) {
				if (candles[i-1].timeStamp - offset != candles[i].timeStamp) {
					cout << "CANDLE MISSING" << endl;
					cout << "Time frame: " << timeFrame << endl;
					cout << "Offset: " << offset << endl;
					cout << "Difference: " << candles[i].timeStamp - candles[i-1].timeStamp << endl;
					cout << "Candle i-1 timestamp: " << candles[i-1].timeStamp << endl;
					cout << "Candle i timestamp: " << candles[i].timeStamp << endl;

					throw std::runtime_error("Candles are missing");
				}
			}
		}

		Candle getHighestCandle() const {
			Candle highestCandle = candles[0];
			for(const auto& candle : candles) {
				if(candle.high > highestCandle.high) {
					highestCandle = candle;
				}
			} return highestCandle;
		}

		Candle getLowestCandle() const {
			Candle lowestCandle = candles[0];
			for(const auto& candle : candles) {
				if(candle.low < lowestCandle.low) {
					lowestCandle = candle;
				}
			} return lowestCandle;
		}

		int getGreenCandles() {
			int greenCandles = 0;
			for(const auto& candle : candles) {
				if(candle.green) {
					greenCandles++;
				}
			} return greenCandles;
		}

		int getRedCandles() {
			int redCandles = 0;
			for(const auto& candle : candles) {
				if(candle.red) {
					redCandles++;
				}
			} return redCandles;
		}

		int getDojiCandles() {
			int dojiCandles = 0;
			for(const auto& candle : candles) {
				if(candle.doji) {
					dojiCandles++;
				}
			} return dojiCandles;
		}

		double getGreenRedRatio() {
			return (double)getGreenCandles() / (double)getRedCandles();
		}

		double getGreenCandlePercent() {
			int greenTrendingStrength = 0;
			for(const auto& candle : candles) {
				if(candle.green) {
					greenTrendingStrength += 1;
				}
			} return greenTrendingStrength / candles.size();
		}

		double getRedCandlePercent() {
			int redTrendingStrength = 0;
			for(const auto& candle : candles) {
				if(candle.red) {
					redTrendingStrength += 1;
				}
			} return redTrendingStrength / candles.size();
		}

		double getCloseAbovePrevClosePercent() {
			int closeAbovePrevClose = 0;
			for(size_t i = 1; i < candles.size(); ++i) {
				if(candles[i].close > candles[i-1].close) {
					closeAbovePrevClose += 1;
				}
			} return closeAbovePrevClose / candles.size(); //todo, bc close above prev close counts sequential it will always be one less then the candles.size
		}

		string printStats() {
			string stats = "";
			stats += "Time frame: " + timeFrame + "\n";
			stats += "Num candles: " + std::to_string(candles.size()) + "\n";
			stats += "Highest candle: " + std::to_string(getHighestCandle().high) + "\n";
			stats += "Lowest candle: " + std::to_string(getLowestCandle().low) + "\n";
			stats += "Green candles: " + std::to_string(getGreenCandles()) + "\n";
			stats += "Red candles: " + std::to_string(getRedCandles()) + "\n";
			stats += "Doji candles: " + std::to_string(getDojiCandles()) + "\n";
			stats += "Green red ratio: " + std::to_string(getGreenRedRatio()) + "\n";
			stats += "Green candle percent: " + std::to_string(getGreenCandlePercent()) + "\n";
			stats += "Red candle percent: " + std::to_string(getRedCandlePercent()) + "\n";
			stats += "Close above prev close percent: " + std::to_string(getCloseAbovePrevClosePercent()) + "\n";
			return stats;
		}
};