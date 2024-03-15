#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Classes/TradingPair.cpp"
#include "Classes/Time.cpp"

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
	ofstream outFile("main log.txt");
	outFile << "starting main.cpp..." << endl;

	Time time;

	TradingPair 
	
	while(true) {
		time.start();

		string prices = runCommand("node data/mexc-pair-prices.js");

		istringstream iss(prices);

		string symbol;
		double price;
		while (iss >> symbol >> price) {
			string tradingPairName = symbol;
			double currentPrice = price;
			cout << tradingPairName << " : " << currentPrice << endl;
		}

		time.end();
		outFile << "duration :" << time.getDuration() << "s" << endl;

		int sleepTime = 300; // (x / 60) mins
		outFile << "sleeping for " << time.sleep(sleepTime) << " seconds..." << endl;
	}

	return 0;
}