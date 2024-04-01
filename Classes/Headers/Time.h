#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include <string>

class Time {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

public:
	Time();

	double start();

	double end();

	double getDuration();

	void sleep(double seconds);

	int now();

	std::string getGMTTime();

	std::string getUnixTime();

	std::string getMSTTime();

	double getElapsedTime(time_t startTime);
};

#endif // TIME_H
