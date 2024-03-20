#ifndef Liquidity_CPP
#define Liquidity_CPP

#include <string>
#include <cstdlib>

#include "../nlohmann/json.hpp"
#include "RunCommand.cpp" 
#include "Log.cpp"

using namespace std;
using json = nlohmann::json;

class Liquidity { 

private:
	string runApiCommand(const string& command) {
		string cmd = "node " + command;
		return runCommand(cmd.c_str());
	}

public:
	Liquidity() {
	}

	double getBidAskQty(string pairName) {
		string response = runApiCommand("/home/johnsmith/Trading/Algorithmic-Trading/mexc-api/book-ticker.js " + pairName);
		try {
			json j = json::parse(response);

			string tempBidQty = j["bidQty"];
			double bidQty = stod(tempBidQty);

			string tempAskQty = j["askQty"];
			double askQty = stod(tempAskQty);

			string tempBidPrice = j["bidPrice"];
			double bidPrice = stod(tempBidPrice);

			string tempAskPrice = j["askPrice"];
			double askPrice = stod(tempAskPrice);

			return (bidQty * bidPrice) + (askQty * askPrice);

		} catch (json::parse_error& e) {
			Log::log("JSON parse error in Liquidity get BidAskQty: " + string(e.what()));
			return -1;
		}
	}

	double get24hrVolume(string pairName) {
		string response = runApiCommand("/home/johnsmith/Trading/Algorithmic-Trading/mexc-api/ticker24hr.js " + pairName);
		try {
			json j = json::parse(response);
			string volume	= j["volume"];
			return stod(volume);
		} catch (json::parse_error& e) {
			Log::log("JSON parse error in Liquidity get 24hrVolume: " + string(e.what()));
			return -1;
		}
	}
};

#endif