#ifndef Log_CPP
#define Log_CPP

#include "Time.cpp"
#include <fstream>
#include <sstream>

using namespace std;

class Log {
private:
	static string filename;

public:
	static void log(string log) {
		ofstream file(filename, ios_base::app);
		file << log << "\n";
	}

	static void LogWithTimestamp(string log) {
		Time time;
		ofstream file(filename, ios_base::app);
		string gmtTime = time.getGMTTime();
		string unixTime = time.getUnixTime();
		string mstTime = time.getMSTTime();

		file << log << "\n"
			<< " | GMT Time: " << gmtTime << " | Unix Time: " << unixTime << " | MST Time: " << mstTime << " |\n";
	}
};

string Log::filename = "log.txt";

#endif