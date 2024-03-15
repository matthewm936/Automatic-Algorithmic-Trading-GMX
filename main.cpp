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
		auto start = std::chrono::system_clock::now();
		std::time_t start_time = std::chrono::system_clock::to_time_t(start);
		outFile << "starting price grabs at: " << std::put_time(std::localtime(&start_time), "%F %T") << std::endl;

		string jsOutput = runCommand("node data/mexc-pair-prices.js");
		// nlohmann::json j = nlohmann::json::parse(jsOutput);

		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		outFile << "finished price grabs at: " << std::put_time(std::localtime(&end_time), "%F %T") << std::endl;

		outFile << "time taken: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds" << std::endl;

		int sleepTime = 1;
		outFile << "sleeping for " << sleepTime << " seconds..." << std::endl;
		this_thread::sleep_for(std::chrono::seconds(sleepTime));
	}

	return 0;
}