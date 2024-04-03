#include <ctime>
#include <string>
#include <unordered_map>

#include "Candlesticks.cpp"

using std::string;
using std::unordered_map;

class Token {
	public:
		string token;
		unordered_map<string, Candlesticks> timeframeCandlesticks;
		
		Token(string token) {
			this->token = token;
		}

		void initCandlesticks(string timeFrame, int maxNumCandles) {
			timeframeCandlesticks[timeFrame] = Candlesticks(timeFrame, maxNumCandles);
		}

		

		
};
