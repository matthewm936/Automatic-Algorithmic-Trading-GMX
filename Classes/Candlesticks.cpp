#include "Candle.cpp"

#include <deque>
#include <string>
#include <iostream>

using std::string;
using std::deque;

int MAX_NUM_CANDLES = 50;

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

		void addCandle(Candle candle) { // todo: this has some unexpected problems, bc of how the api data gets fed into addCandle
		// loop through and find it candle already exists

		// if not then

		// if candles.size() == 0, push_front(candle)

		// loop and find its exact spot?
		
			if(candles.size() == 0) {
				candles.push_front(candle);
			} else if(candles.size() == 1) {
				if(candles[0].timeStamp < candle.timeStamp) {
					candles.push_front(candle);
				} else if(candles[0].timeStamp > candle.timeStamp) {
					candles.push_back(candle);
				} else if(candles[0].timeStamp == candle.timeStamp) {
					std::cout << "Candle with same timestamp already exists" << std::endl;
				}
			} else if(candles.size() > 1) {
				if(candles[0].timeStamp < candle.timeStamp) {
					candles.push_front(candle);
				} else if(candles[0].timeStamp > candle.timeStamp) {
					candles.push_back(candle);
				} else if(candles[0].timeStamp == candle.timeStamp) {
					std::cout << "Candle with same timestamp already exists" << std::endl;
				}
			}
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