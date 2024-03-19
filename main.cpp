#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <fstream>
#include <cstdio>
#include <map>
#include <string>

#include "Classes/TradingPairs.cpp"
#include "Classes/Time.cpp"
#include "Classes/Log.cpp"

using namespace std;

map<string, Position> positions;

std::string runCommand(const char *cmd) {
	Log::log("cmd exec " + string(cmd));

	std::array<char, 128> buffer;
	std::string result;
	#ifdef _WIN32
		std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen((std::string(cmd) + " 2>&1").c_str(), "r"), _pclose);
	#else
		std::unique_ptr<FILE, decltype(&pclose)> pipe(popen((std::string(cmd) + " 2>&1").c_str(), "r"), pclose);
	#endif
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

int main() {
	Time time;
	Log::LogWithTimestamp("MAIN.cpp Started");

	TradingPairs pairs; 
	string prices = runCommand("node mexc-api/pair-prices.js");


	istringstream iss(prices);

	string symbol;
	double price;
	while (iss >> symbol >> price) {
		pairs.addPair(price, symbol);
	}

	Log::LogWithTimestamp("Init pair prices of size: " + to_string(prices.size()) + " to pairs");

	while(true) {
		string prices = runCommand("node mexc-api/pair-prices.js");

		istringstream iss(prices);

		string symbol;
		double price;
		while (iss >> symbol >> price) {
			pairs.updatePair(price, symbol);

			if (positions.find(symbol) != positions.end()) {
				positions[symbol].currentPrice = price;

				double profitLoss = (positions[symbol].currentPrice - positions[symbol].entryPrice) / positions[symbol].entryPrice;
				positions[symbol].profitLoss = profitLoss;

				if(profitLoss > 0.05) {
					Log::LogWithTimestamp("MAIN.cpp, " + symbol + " profit/loss: " + to_string(positions[symbol].profitLoss));
					// string command = "node /mexc-api/sell.js " + symbol + " SELL " + " 20";
					// const char* cmd = command.c_str();
					// system(cmd);
				}
				if(profitLoss < -0.05) {
					Log::LogWithTimestamp("MAIN.cpp, " + symbol + " profit/loss: " + to_string(positions[symbol].profitLoss));
					// string command = "node /mexc-api/sell.js " + symbol + " SELL " + " 20";
					// const char* cmd = command.c_str();
					// system(cmd);
				}
			}	
		}


		Log::logNoNewline("sleep ");
		int sleepTimeMins = 1;
		int sleepTime = sleepTimeMins * 60;
		time.sleep(sleepTime);
	}

	return 0;
}