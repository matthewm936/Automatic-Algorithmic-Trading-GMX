#include <ctime>
#include <iostream>

class Candle {
	private:
		void checkCorrectness() { // according to prelim testing on candlestick data, this api is returning incorrect data of the open can be the low or high
			if(high < low) { // so switching this to just catch incorrect if breaks by definition
				throw std::runtime_error("ERROR: high  not greater than low");
			} 
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


			// std::cout << "Candle object created with time stamp: " << timeStamp << std::endl;
			// std::cout << "Open: " << open << std::endl;
			// std::cout << "High: " << high << std::endl;
			// std::cout << "Low: " << low << std::endl;
			// std::cout << "Close: " << close << std::endl;

			checkCorrectness();
		}		
};
