#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std;

string runCommand(const string& command) {
	char buffer[128];
	string result = "";
	FILE* pipe = popen(command.c_str(), "r");
	while (fgets(buffer, sizeof buffer, pipe) != NULL) {
		result += buffer;
	}
	pclose(pipe);
	return result;
}

int main() {
	ofstream outFile("main log.txt");

	outFile << "starting main.cpp..." << endl;
	
	while(true) {
		string line = "";
		ifstream inFile("data/mexcpairs_filtered");
		while(getline(inFile, line)) {
			string jsOutput = runCommand("node data/mexc-price-ticker-api.js " + line);
		}

		int sleepTime = 10;
		outFile << "sleeping for " << sleepTime << " seconds..." << endl;
		this_thread::sleep_for(std::chrono::seconds(sleepTime));
	}

	return 0;
}