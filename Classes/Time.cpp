#ifndef Time_cpp
#define Time_cpp

#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include <string>
#include <iostream>

class Time {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

public:
	Time() {
		// std::cout << "Time object created" << std::endl;
	};

	double start() {
		std::cout << "Time::start() called" << std::endl;

		startTime = std::chrono::high_resolution_clock::now();
		auto epoch = startTime.time_since_epoch();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		return seconds.count();
	}

	double end() {
		std::cout << "Time::end() called" << std::endl;

		endTime = std::chrono::high_resolution_clock::now();
		auto epoch = endTime.time_since_epoch();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		return seconds.count();
	}

	double getDuration() {
		std::chrono::duration<double> duration = endTime - startTime;
		return duration.count();
	}

	std::string sleep() {
		double sleepTimeMins = 1;
		double sleepTime = sleepTimeMins * 15 - getDuration(); // 15 second sleeptime
		if (sleepTime < 0) sleepTime = 0; 

		std::string output = "Time::sleep() called for " + std::to_string(sleepTime) + "s at MST " + getMSTTime() + " and Unix " + getUnixTime();

		std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));

		return output;
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

	std::string getMSTTime() {
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);

		// Adjust to MST (which is 7 hours behind UTC)
		now_c -= 7 * 60 * 60;

		std::stringstream ss;
		ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");

		return ss.str();
	}

	double getElapsedTime(time_t startTime) {
		auto now = std::chrono::high_resolution_clock::now();
		auto epoch = now.time_since_epoch();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		return seconds.count() - startTime;
	}
};

#endif