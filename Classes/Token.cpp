#include <ctime>
#include <string>
#include <unordered_map>
#include <iostream>

#include "Candlesticks.cpp"

using std::string;
using std::unordered_map;

class Token {
	private:


	public:
		string token;
		unordered_map<string, Candlesticks> timeframeCandlesticks;
		
		Token() {
			// std::cout << "Token object created" << std::endl;
		};
		
		Token(string token) {
			this->token = token;

			// std::cout << "Token object created with token: " << token << std::endl;
		}

		void addCandlesticks(Candlesticks candlesticks) {
			timeframeCandlesticks[candlesticks.getTimeFrame()] = candlesticks;

			// std::cout << "Candlesticks added to token: " << token << std::endl;
			// std::cout << "Time frame: " << candlesticks.getTimeFrame() << std::endl;
		}

		Candlesticks& getCandlesticks(string timeframe) {
			return timeframeCandlesticks[timeframe];
		}

		string toStringToken() {
			string output;

			output += "Token: " + token + "\n";
			output += "Timeframes: \n";

			for(const auto& [key, value] : timeframeCandlesticks) {
				output += "Timeframe: " + key + "\n";
				output += "Current Price: " + std::to_string(timeframeCandlesticks[key].getCurrentPrice()) + "\n";
			}
			return output;
		}
};
