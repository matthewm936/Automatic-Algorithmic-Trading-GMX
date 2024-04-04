#include <ctime>
#include <iostream>

class Candle {
	private:
		void checkCorrectness() {
			if(high < low || high < open || high < close || low > open || low > close) {
				throw std::runtime_error("a high low close or open doenst follow");
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

			checkCorrectness();

			// std::cout << "Candle object created with time stamp: " << timeStamp << std::endl;
			// std::cout << "Open: " << open << std::endl;
			// std::cout << "High: " << high << std::endl;
			// std::cout << "Low: " << low << std::endl;
			// std::cout << "Close: " << close << std::endl;
		}		
};
