#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>

using namespace std;

string exec(const char* cmd) {
	array<char, 128> buffer;
	string result;
	unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		throw runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

int main() {
	cout << "starting main..." << endl;
	
	while(true) {
		cout << "Running LiveTradingData.js..." << endl;

		string jsonString = exec("node LiveTradingData.js");

		// cout << "JSON String: " << jsonString << endl;

		nlohmann::json j = nlohmann::json::parse(jsonString);

		// cout << "parsed JSON: " << j << endl;

		
		system("python /ServerLogs/pi-prices-notification.py");

		cout << "Sleeping for 600 seconds..." << endl;
		this_thread::sleep_for(chrono::seconds(600));
	}
	return 0;
}