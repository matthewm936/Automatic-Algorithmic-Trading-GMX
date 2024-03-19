#ifndef Time_CPP
#define Time_CPP

#include <chrono>
#include <ctime>
#include <iomanip>

class Time {

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

public:
	Time() {
	}
	
	double start() {
		startTime = std::chrono::high_resolution_clock::now();
		auto epoch = startTime.time_since_epoch();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		return seconds.count();
	}

	double end() {
		endTime = std::chrono::high_resolution_clock::now();
		auto epoch = endTime.time_since_epoch();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		return seconds.count();
	}

	double getDuration() {
		std::chrono::duration<double> duration = endTime - startTime;
		return duration.count();
	}

	int sleep(int seconds) {
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
		return seconds;
	}

	int now() {
		auto now = std::chrono::high_resolution_clock::now();
		auto epoch = now.time_since_epoch();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		return seconds.count();
	}

	std::string getGMTTime() {
		auto now = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		std::tm* gmt_time = std::gmtime(&now_time);
		char buffer[50];
		std::strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", gmt_time);
		return std::string(buffer);
	}

	std::string getUnixTime() {
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
		return std::to_string(seconds.count());
	}
};

#endif