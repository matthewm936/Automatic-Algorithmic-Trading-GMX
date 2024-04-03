#include <ctime>
#include <iostream>

class Candle {
	public:
		time_t timeStamp;
		double open;
		double high;
		double low;
		double close;

		Candle() {
			// std::cout << "Candle object created" << std::endl;
		};
		
		// todo:: the order of these mihgt be wrong, check gmx josn result
		Candle(time_t timeStamp, double open, double high, double low, double close) {
			this->timeStamp = timeStamp;
			this->open = open;
			this->high = high;
			this->low = low;
			this->close = close;

			// std::cout << "Candle object created with time stamp: " << timeStamp << std::endl;
			// std::cout << "Open: " << open << std::endl;
			// std::cout << "High: " << high << std::endl;
			// std::cout << "Low: " << low << std::endl;
			// std::cout << "Close: " << close << std::endl;
		}		
};
