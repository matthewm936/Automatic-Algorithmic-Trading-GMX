#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>

using namespace std;

int main() {
	cout << "starting main.cpp..." << endl;
	
	while(true) {


		cout << "Sleeping for 600 seconds..." << endl;
		this_thread::sleep_for(std::chrono::seconds(600));
	}
	return 0;
}