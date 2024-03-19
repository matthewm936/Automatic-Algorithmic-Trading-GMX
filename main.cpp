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
#include "Classes/RunCommand.cpp"

using namespace std;

map<string, Position> g_positions;

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

			if (g_positions.find(symbol) != g_positions.end()) {
				g_positions[symbol].currentPrice = price;

				double profitLoss = (g_positions[symbol].currentPrice - g_positions[symbol].entryPrice) / g_positions[symbol].entryPrice;
				g_positions[symbol].profitLoss = profitLoss;

				if(profitLoss > 0.05) {
					Log::LogWithTimestamp("MAIN.cpp, " + symbol + " profit/loss: " + to_string(g_positions[symbol].profitLoss));
					// string command = "node /mexc-api/sell.js " + symbol + " SELL " + " 20";
					// const char* cmd = command.c_str();
					// system(cmd);
				}
				if(profitLoss < -0.05) {
					Log::LogWithTimestamp("MAIN.cpp, " + symbol + " profit/loss: " + to_string(g_positions[symbol].profitLoss));
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