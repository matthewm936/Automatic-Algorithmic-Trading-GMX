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
			cout << "bidQty: " << j["bidQty"] << endl;
		} catch (json::parse_error& e) {
			Log::log("JSON parse error in Liquidity get BidAskQty: " + string(e.what()));
			cout << "JSON parse error: " << e.what() << endl;
		}

		return -1;

		// double bidQty = j["bidQty"].get<double>();
		// double askQty = j["askQty"].get<double>();
		// double bidPrice = j["bidPrice"].get<double>();
		// double askPrice = j["askPrice"].get<double>();

		// return bidQty * bidPrice + askQty * askPrice;
	}

	double get24hrVolume(string pairName) {
		string response = runApiCommand("/home/johnsmith/Trading/Algorithmic-Trading/mexc-api/ticker24hr.js " + pairName);
		json j = json::parse(response);

		return j["volume"].get<double>();
	}
};

#endif