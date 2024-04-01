#include <string>
#include <cstdlib>

#include "../nlohmann/json.hpp"
#include "Headers/Log.h"
#include "Headers/RunCommand.h"
#include "Headers/Liquidity.h"

using namespace std;
using json = nlohmann::json;

namespace Liquidity {

	string runApiCommand(const string& command) {
		string cmd = "node " + command;
		return runCommand(cmd.c_str());
	}

	json getJsonResponse(const string& command) {
		string response = runApiCommand(command);
		try {
			return json::parse(response);
		} catch (json::parse_error& e) {
			Log::log("JSON parse error in Liquidity: " + string(e.what()));
			throw;
		}
	}

	double getBidAskQty(string pairName) {
		json j = getJsonResponse("/home/johnsmith/Trading/Algorithmic-Trading/mexc-api/book-ticker.js " + pairName);

		double bidQty = stod(j["bidQty"].get<string>());
		double askQty = stod(j["askQty"].get<string>());
		double bidPrice = stod(j["bidPrice"].get<string>());
		double askPrice = stod(j["askPrice"].get<string>());

		return (bidQty * bidPrice) + (askQty * askPrice);
	}

	double get24hrVolume(string pairName) {
		json j = getJsonResponse("/home/johnsmith/Trading/Algorithmic-Trading/mexc-api/ticker24hr.js " + pairName);
		return stod(j["volume"].get<string>());	
	}
}