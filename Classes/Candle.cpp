#include <ctime>
#include <iostream>

using std::cout;
using std::endl;

class Candle {
	private:
		validateAndCorrectCandleValues() {
			if (high < low) {
				Log::logError("CANDLE ERROR; High: " + to_string(high) + " Low: " + to_string(low) + " low larger by:" + to_string(high - low));
				
			}
			if (close < low) {
				Log::logError("CANDLE ERROR; Close: " + to_string(close) + " Low: " + to_string(low) + " close less by:" + to_string(low - low));
				low = close;
				Log::logError("Setting low to the close: " + to_string(close) + " Low: " + to_string(low));
			}
			if (close > high) {
				Log::logError("CANDLE ERROR; Close: " + to_string(close) + " High: " + to_string(high) + " close larger by:" + to_string(close - high));
				high = close;
				Log::logError("Setting high to the close: " + to_string(close) + " High: " + to_string(high));
			}
			if (open < low) {
				Log::logError("CANDLE ERROR; Open: " + to_string(open) + " Low: " + to_string(low) + " open less by:" + to_string(low - open));
				low = open;
				Log::logError("Setting low to the open: " + to_string(open) + " Low: " + to_string(low));
			}
			if (open > high) {
				Log::logError("CANDLE ERROR; Open: " + to_string(open) + " High: " + to_string(high) + " open larger by:" + to_string(open - high));
				high = open;
				Log::logError("Setting high to the open: " + to_string(open) + " High: " + to_string(high));
			}
		}

		calculateAndValidateWickRatioIndex() {
			WickRatioIndex = (close - open) / (high - low);
			if(WickRatioIndex > 1 || WickRatioIndex < -1) {
				Log::logError("CANDLE ERROR; wick ratio index Meter: " + to_string(WickRatioIndex));
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

			validateAndCorrectCandleValues();
			calculateAndValidateWickRatioIndex();
			
			if(close > open) {
				green = true;
			} else if(close < open) {
				red = true;
			} else if(close == open) {
				doji = true;
			}

			OpenClosePercentChange = (close - open) / open;
			HighLowPercentChange = (high - low) / low;
		}		

		string toString() {
			return "Candle: " + std::to_string(timeStamp) + " Open: " + std::to_string(open) + " High: " + std::to_string(high) + " Low: " + std::to_string(low) + " Close: " + std::to_string(close);
		}
};
