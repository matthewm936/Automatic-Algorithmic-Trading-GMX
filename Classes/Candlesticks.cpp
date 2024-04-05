#include "Candle.cpp"

#include <deque>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using std::string;
using std::deque;

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

			std::cout << "Candlesticks object created with time frame: " << timeFrame << std::endl;
		}

		void addCandle(Candle candle) { 
			for(const auto& c : candles) {
				if(c.timeStamp == candle.timeStamp) {
					std::cout << "Candle with same timestamp already exists" << std::endl;
					return;
				}
			}
			// if not then
			auto pos = std::find_if(candles.begin(), candles.end(), [&candle](const Candle& c) {
				return c.timeStamp < candle.timeStamp;
			});
			candles.insert(pos, candle); // todo: is this the best way to insert a candle into the deque? or should I just
		
			if(candles.size() > maxNumCandles) {
				candles.pop_back();
			}
		}

		string getTimeFrame() {
			return timeFrame;
		}

		deque<Candle> getCandles() {
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
				if (candles[i-1].timeStamp + offset != candles[i].timeStamp) {
					throw std::runtime_error("Candles are missing");
				}
			}
		}

		Candle getHighestCandle() {
			Candle highestCandle = candles[0];
			for(const auto& candle : candles) {
				if(candle.high > highestCandle.high) {
					highestCandle = candle;
				}
			} return highestCandle;
		}

		Candle getLowestCandle() {
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
};