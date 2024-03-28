#ifndef Log_CPP
#define Log_CPP

#include <fstream>
#include <sstream>

#include "Time.cpp"

using namespace std;

// All errors get logged and automatically emailed

class Log {
private:
	static string baseFilename;
	static string currentFilename;

	static void checkLogFile() {
		Time time;
		string currentDate = time.getGMTTime().substr(0, 10); // Get the current date in 'YYYY-MM-DD' format

		if (currentFilename != baseFilename + currentDate) {
			// A new day has started, so rotate the log file
			string oldFilename = currentFilename;
			currentFilename = baseFilename + currentDate;

			// Delete the log file from two days ago
			if (!oldFilename.empty()) {
				filesystem::remove(oldFilename);
			}
		}
	}

	static void email(string message) {
		string command = "python3 /home/johnsmith/Trading/Algorithmic-Trading/Notifications/pi-email-message.py \"" + message + "\"";
		system(command.c_str());
	}

public:
	static void logNoNewline(string log) {
		checkLogFile();
		ofstream file(currentFilename, ios_base::app);
		file << log;
	
	}
	static void log(string log) {
		checkLogFile();
		ofstream file(currentFilename, ios_base::app);
		file << log << "\n";
	}

	static void LogWithTimestamp(string log) {
		checkLogFile();
		Time time;
		ofstream file(currentFilename, ios_base::app);
		string gmtTime = time.getGMTTime();
		string unixTime = time.getUnixTime();

		file << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << "] " << log << "\n";
	}

	static void logError(string log) {
		Time time;
		string gmtTime = time.getGMTTime();
		string unixTime = time.getUnixTime();

		ofstream file("error_log.txt", ios_base::app);
		file << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << "] " << log << "\n";

		email(log);
	}
};

string Log::baseFilename = "log";
string Log::currentFilename = "";
#endif