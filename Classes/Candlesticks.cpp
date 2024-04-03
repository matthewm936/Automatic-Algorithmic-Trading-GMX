#include "Candle.cpp"

#include <deque>
#include <string>

using std::string;
using std::deque;

int MAX_NUM_CANDLES = 20;

class Candlesticks {
	private:
		std::deque<Candle> candles;
		std::string timeFrame = "";
		deque<Candle>::size_type maxNumCandles = MAX_NUM_CANDLES;
		
	public:
		Candlesticks() {};
		
		Candlesticks(string timeFrame) {
			this->timeFrame = timeFrame;
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
