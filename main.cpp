#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Classes/TradingPairs.cpp"
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
	Time time;
	ofstream outFile("main log.txt");

	outFile << "starting main.cpp at " << time.now() << endl;

	TradingPairs pairs; 
	string prices = runCommand("node data/mexc-pair-prices.js");

	istringstream iss(prices);

	string symbol;
	double price;
	while (iss >> symbol >> price) {
		pairs.addPair(price, symbol);
	}
	outFile << pairs.getNumPairs() << " pairs added" << endl;

	while(true) {
		time.start();

		string prices = runCommand("node data/mexc-pair-prices.js");

		istringstream iss(prices);

		string symbol;
		double price;
		while (iss >> symbol >> price) {
			pairs.updatePair(price, symbol);	
		}

		time.end();
		outFile << "duration: " << time.getDuration() << "s" << endl;

		int sleepTimeMins = 1;
		int sleepTime = sleepTimeMins * 60;
		outFile << "sleeping for " << sleepTime << "s/" << sleepTime/60 << "mins" << endl;
		time.sleep(sleepTime);
	}

	return 0;
}