#include "Candle.cpp"

#include <deque>
#include <string>

using std::string;
using std::deque;

class Candles {
	private:
		std::deque<Candle> candles;
		std::string timeFrame = "";
		deque<Candle>::size_type maxNumCandles;
		
	public:
		Candles(string timeFrame, int maxNumCandles) {
			this->timeFrame = timeFrame;
			this->maxNumCandles = maxNumCandles;
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
