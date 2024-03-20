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
#include "Classes/RunCommand.cpp"
#include "Classes/Liquidity.cpp"

using namespace std;

map<string, Position> g_positions;

int main() {
	Time time;
	Log::LogWithTimestamp("MAIN.cpp Started");

	TradingPairs pairs; 
	
	runCommand("node mexc-api/ticker24hrALL.js");

	ifstream i("prices.json");
	nlohmann::json j;
	i >> j;

	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		string symbol = it->at("symbol").get<string>();

		string priceStr = it->at("lastPrice").get<string>();
		double lastPrice = stod(priceStr);

		string askPriceStr = it->at("askPrice").get<string>();
		double askPrice = stod(askPriceStr);

		string bidPriceStr = it->at("bidPrice").get<string>();
		double bidPrice = stod(bidPriceStr);

		string askQtyStr = it->at("askQty").get<string>();
		double askQty = stod(askQtyStr);

		string bidQtyStr = it->at("bidQty").get<string>();
		double bidQty = stod(bidQtyStr);

		string volumeStr = it->at("volume").get<string>();
		double volume = stod(volumeStr);

		string quoteVolumeStr = it->at("quoteVolume").get<string>();
		double quoteVolume = stod(quoteVolumeStr);

		pairs.addPair(lastPrice, symbol, askPrice, bidPrice, askQty, bidQty, volume, quoteVolume);
	}

	Log::log("main.cpp, pairs has " + pairs.getNumPairs() + " of pairs");
	
	// Log::log("Average liquidity for USDT pairs: " + to_string(averageLiquidty / pairs.size()));
	// Log::log("Total liquidity for USDT pairs: " + to_string(averageLiquidty));

	// Log::LogWithTimestamp("Init pair prices of size: " + to_string(prices.size()) + " to pairs");

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