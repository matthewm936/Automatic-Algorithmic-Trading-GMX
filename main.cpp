#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdio>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

#include "Classes/TradingPairs.cpp"
#include "Classes/Time.cpp"
#include "Classes/Log.cpp"
#include "Classes/Liquidity.cpp"

map<string, Position> g_positions;

double getDoubleFromJson(nlohmann::json::iterator& it, const string& key) {
	string str = it->at(key).get<string>();
	return stod(str);
}

int main() {
	Time time;
	Log::LogWithTimestamp("MAIN.cpp Started");

	TradingPairs pairs; 
	
	runCommand("node mexc-api/ticker24hrALL.js");

	ifstream i("prices.json");
	nlohmann::json j;
	i >> j;

	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		std::string symbol = it->at("symbol").get<std::string>();
		double lastPrice = getDoubleFromJson(it, "lastPrice");
		double askPrice = getDoubleFromJson(it, "askPrice");
		double bidPrice = getDoubleFromJson(it, "bidPrice");
		double askQty = getDoubleFromJson(it, "askQty");
		double bidQty = getDoubleFromJson(it, "bidQty");
		double volume = getDoubleFromJson(it, "volume");
		double quoteVolume = getDoubleFromJson(it, "quoteVolume");

		pairs.addPair(lastPrice, symbol, askPrice, bidPrice, askQty, bidQty, volume, quoteVolume);
	}

	Log::log("main.cpp, TradingPairs pairs has " + to_string(pairs.getNumPairs()) + " of trading pairs");

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
					Log::LogWithTimestamp("MAIN.cpp, " + symbol + " profit/loss: " + to_string(g_positions[symbol].profitLoss) + " after" + to_string(time.getElapsedTime(g_positions[symbol].entryTime)) + " seconds");
					// string command = "node /mexc-api/sell.js " + symbol + " SELL " + " 20";
					// const char* cmd = command.c_str();
					// system(cmd);
					g_positions.erase(symbol);
				}
				if(profitLoss < -0.05) {
					Log::LogWithTimestamp("MAIN.cpp, " + symbol + " profit/loss: " + to_string(g_positions[symbol].profitLoss) + " after" + to_string(time.getElapsedTime(g_positions[symbol].entryTime)) + " seconds");
					// string command = "node /mexc-api/sell.js " + symbol + " SELL " + " 20";
					// const char* cmd = command.c_str();
					// system(cmd);
					g_positions.erase(symbol);
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