#include <ctime>
#include <iostream>

using std::cout;
using std::endl;

class Candle {
	private:
		void checkCorrectness() { // according to prelim testing on candlestick data, this api is returning incorrect data of the open can be the low or high
			if(high < low) { // so switching this to just catch incorrect if breaks by definition
				Log::logError("CANDLE ERROR; High: " + std::to_string(high) + " Low: " + std::to_string(low));
				throw std::runtime_error("ERROR: high  not greater than low");
			} 
			// if(close < low) {
			// 	Log::logError("CANDLE ERROR; Close: " + std::to_string(close) + " Low: " + std::to_string(low));
			// 	// throw std::runtime_error("ERROR: close not greater than low");
			// }
			// if(close > high) {
			// 	Log::logError("CANDLE ERROR; Close: " + std::to_string(close) + " High: " + std::to_string(high));
			// 	// throw std::runtime_error("ERROR: close not less than high");
			// }
			// if(open < low) {
			// 	Log::logError("CANDLE ERROR; Open: " + std::to_string(open) + " Low: " + std::to_string(low));
			// 	// throw std::runtime_error("ERROR: open not greater than low");
			// }
			// if(open > high) {
			// 	Log::logError("CANDLE ERROR; Open: " + std::to_string(open) + " High: " + std::to_string(high));
			// 	// throw std::runtime_error("ERROR: open not less than high");
			// }
		}

	public:
		time_t timeStamp;
		double open;
		double high;
		double low;
		double close;

		bool green = false;
		bool red = false;
		bool doji = false;

		Candle() {
			// std::cout << "Candle object created" << std::endl;
		};
		
		Candle(time_t timeStamp, double open, double high, double low, double close) {
			this->timeStamp = timeStamp;
			this->open = open;
			this->high = high;
			this->low = low;
			this->close = close;

			if(close > open) {
				green = true;
			} else if(close < open) {
				red = true;
			} else if(close == open) {
				doji = true;
			}

			checkCorrectness();
		}		

		string toString() {
			return "Candle: " + std::to_string(timeStamp) + " Open: " + std::to_string(open) + " High: " + std::to_string(high) + " Low: " + std::to_string(low) + " Close: " + std::to_string(close);
		}
};
