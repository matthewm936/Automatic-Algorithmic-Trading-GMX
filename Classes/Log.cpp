#ifndef Log_CPP
#define Log_CPP

#include "Time.cpp"
#include <fstream>
#include <sstream>

class Log {
private:
	Time time;
	std::string filename;

public:
	Log() : filename("log.txt") {
	}

	void addLog(std::string log) {
		std::ofstream file(filename, std::ios_base::app);
		file << log << "\n";
	}

	void addLogWithTimestamp(std::string log) {
		std::ofstream file(filename, std::ios_base::app);
		std::string gmtTime = time.getGMTTime();
		std::string unixTime = time.getUnixTime();
		std::string mstTime = time.getMSTTime();

		file << log << "\n"
			<< " | GMT Time: " << gmtTime << " | Unix Time: " << unixTime << " | MST Time: " << mstTime << " |\n";
	}
};

#endif