#include "Candle.cpp"

#include <deque>
#include <string>
#include <iostream>

using std::string;
using std::deque;

int MAX_NUM_CANDLES = 20;

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

		void addCandle(Candle candle) {
			candles.push_front(candle);
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
};
