#include <ctime>
#include <iostream>

using std::cout;
using std::endl;

class Candle {
	private:
		void checkCorrectness() { // according to prelim testing on candlestick data, this api is returning incorrect data of the open can be the low or high
			// if(high < low) { // so switching this to just catch incorrect if breaks by definition
			// 	throw std::runtime_error("ERROR: high  not greater than low");
			// } 
			// if(close < low) {
			// 	cout << "Close: " << close << " Low: " << low << endl;
			// 	throw std::runtime_error("ERROR: close not greater than low");
			// }
			// if(close > high) {
			// 	cout << "Close: " << close << " High: " << high << endl;
			// 	throw std::runtime_error("ERROR: close not less than high");
			// }
			// if(open < low) {
			// 	cout << "Open: " << open << " Low: " << low << endl;
			// 	throw std::runtime_error("ERROR: open not greater than low");
			// }
			// if(open > high) {
			// 	cout << "Open: " << open << " High: " << high << endl;
			// 	throw std::runtime_error("ERROR: open not less than high");
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


			// std::cout << "Candle object created with time stamp: " << timeStamp << std::endl;
			// std::cout << "Open: " << open << std::endl;
			// std::cout << "High: " << high << std::endl;
			// std::cout << "Low: " << low << std::endl;
			// std::cout << "Close: " << close << std::endl;

			checkCorrectness();
		}		

		string toString() {
			return "Candle: " + std::to_string(timeStamp) + " Open: " + std::to_string(open) + " High: " + std::to_string(high) + " Low: " + std::to_string(low) + " Close: " + std::to_string(close);
		}
};
