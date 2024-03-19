#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Classes/TradingPairs.cpp"
#include "Classes/Time.cpp"
#include "Classes/Log.cpp"

using namespace std;

std::string runCommand(const char *cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen((std::string(cmd) + " 2>&1").c_str(), "r"), pclose);
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
	Log log;

	log.addLogWithTimestamp("MAIN.cpp");

	TradingPairs pairs; 
	string prices = runCommand("node /mexc-api/pair-prices.js");

	istringstream iss(prices);

	string symbol;
	double price;
	while (iss >> symbol >> price) {
		pairs.addPair(price, symbol);
	}

	while(true) {
		string prices = runCommand("node /mexc-api/pair-prices.js");

		istringstream iss(prices);

		string symbol;
		double price;
		while (iss >> symbol >> price) {
			pairs.updatePair(price, symbol);	
		}

		int sleepTimeMins = 1;
		int sleepTime = sleepTimeMins * 60;
		time.sleep(sleepTime);
	}

	return 0;
}