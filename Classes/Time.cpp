#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include <iostream>

#include "Headers/Time.h"

std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

Time::Time() {}

double Time::start() {
	std::cout << "Time::start() called" << std::endl;

	startTime = std::chrono::high_resolution_clock::now();
	auto epoch = startTime.time_since_epoch();
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
	return seconds.count();
}

double Time::end() {
	std::cout << "Time::end() called" << std::endl;

	endTime = std::chrono::high_resolution_clock::now();
	auto epoch = endTime.time_since_epoch();
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
	return seconds.count();
}

double Time::getDuration() {
	std::chrono::duration<double> duration = endTime - startTime;

	std::cout << "Time::getDuration() called for " << duration.count() << " seconds" << std::endl;
	return duration.count();
}

void Time::sleep(double seconds) {
	std::cout << "Time::sleep() called for s" << seconds << std::endl;

	std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
}

int Time::now() {
	auto now = std::chrono::high_resolution_clock::now();
	auto epoch = now.time_since_epoch();
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
	return seconds.count();
}

std::string Time::getGMTTime() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm* gmt_time = std::gmtime(&now_time);
	char buffer[50];
	std::strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", gmt_time);
	return std::string(buffer);
}

std::string Time::getUnixTime() {
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
	return std::to_string(seconds.count());
}

std::string Time::getMSTTime() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);

	// Adjust to MST (which is 7 hours behind UTC)
	now_c -= 7 * 60 * 60;

	std::stringstream ss;
	ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");

	return ss.str();
}

double Time::getElapsedTime(time_t startTime) {
	auto now = std::chrono::high_resolution_clock::now();
	auto epoch = now.time_since_epoch();
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
	return seconds.count() - startTime;
}
