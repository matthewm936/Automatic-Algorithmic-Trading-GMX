#include <ctime>
#include <iostream>

using std::cout;
using std::endl;

class Candle {
	private:

	public:
		time_t timeStamp;
		double open;
		double high;
		double low;
		double close;

		bool green = false;
		bool red = false;
		bool doji = false;

		double WickRatioIndex; // -1 to 1; 1 being most bullish -1 being more bearish
		
		double OpenClosePercentChange;
		double HighLowPercentChange;

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

			WickRatioIndex = (close - open) / (high - low);
			OpenClosePercentChange = (close - open) / open;
			HighLowPercentChange = (high - low) / low;
		}		

		string toString() {
			return "Candle: " + std::to_string(timeStamp) + " Open: " + std::to_string(open) + " High: " + std::to_string(high) + " Low: " + std::to_string(low) + " Close: " + std::to_string(close);
		}
};
