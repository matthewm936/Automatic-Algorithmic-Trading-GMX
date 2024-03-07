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

		cout << "JSON String: " << jsonString << endl;

		nlohmann::json j = nlohmann::json::parse(jsonString);

		cout << "parsed JSON: " << j << endl;

		// Access values
		// bool canTrade = j["canTrade"];
		// bool canWithdraw = j["canWithdraw"];
		// bool canDeposit = j["canDeposit"];

		// cout << canTrade << endl;
		// cout << canWithdraw << endl;
		// cout << canDeposit << endl;

		// // Access balances
		// for (const auto& balance : j["balances"]) {
		// 	string asset = balance["asset"];
		// 	string free = balance["free"];
		// 	string locked = balance["locked"];
		// 	cout << "Asset: " << asset << ", Free: " << free << ", Locked: " << locked << endl;
		// }

		// // Access permissions
		// for (const auto& permission : j["permissions"]) {
		// 	cout << "Permission: " << permission << endl;
		// }

		cout << "Sleeping for 10 seconds..." << endl;
		this_thread::sleep_for(chrono::seconds(10));
	}
	return 0;
}