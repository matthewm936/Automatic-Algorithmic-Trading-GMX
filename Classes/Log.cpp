#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> 
#include <ctime>
#include <string>
#include <chrono>
#include <filesystem>

#include "Time.cpp"

using std::string;

class Log {
private:
	static string baseFilename;
	static string currentFilename;

	static void checkLogFile();
	
public:
	static void clearLogFiles();
	static void logNoNewline(string log);
	static void log(string log);
	static void logAndEmail(string log);
	static void LogWithTimestamp(string log);
	static void logState(string log, string filename);
	static void logTrade(string log);
	static void logError(string log, bool emailFlag, bool timestampFlag);

	static void email(string header, string log);
};

string Log::baseFilename = "log";
string Log::currentFilename = "";

void Log::checkLogFile() {
	Time time;

	string currentDate = time.getGMTTime().substr(0, 10); 

	if (currentFilename != baseFilename + currentDate) {
		string oldFilename = currentFilename;
		currentFilename = baseFilename + currentDate;

		if (!oldFilename.empty()) {
			std::filesystem::remove(oldFilename);
		}
	}
}

void Log::email(string header, string message) {
	string command = "python3 /home/johnsmith/Trading/Algorithmic-Trading/Notifications/pi-email-message.py \"" + header + "\" \"" + message + "\"";
	system(command.c_str());
}

void Log::clearLogFiles() {
	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::trunc);
	std::ofstream errorFile("error_log.txt", std::ios_base::trunc);
	std::ofstream positions("positions.txt", std::ios_base::trunc);
	std::ofstream tradeLog("trade_log.txt", std::ios_base::trunc);
	
	file.close();
	errorFile.close();
	positions.close();
	tradeLog.close();
}

void Log::logNoNewline(string log) {
	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::app);
	file << log;
}

void Log::log(string log) {
	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::app);
	file << log << "\n";
}

void Log::logAndEmail(string log) {
	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::app);
	file << log << "\n";

	email("regular log", log);
}

void Log::LogWithTimestamp(string log) {
	Time time;

	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::app);
	string gmtTime = time.getGMTTime();
	string unixTime = time.getUnixTime();
	string mstTime = time.getMSTTime();

	file << log << "\n" << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << ", MST Time: " << mstTime << "] \n";
}

void Log::logState(string log, string filename) {
	Time time;

	std::ofstream file(filename, std::ios_base::trunc);

	file << log << "\n";
	file << "-------------- Last Updated MST: " + time.getMSTTime() + " --------------\n";
}

void Log::logTrade(string log) {
	std::ofstream file("trade_log.txt", std::ios_base::app);
	Time time;

	string gmtTime = time.getGMTTime();
	string unixTime = time.getUnixTime();
	string mstTime = time.getMSTTime();

	file << log << "\n" << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << ", MST Time: " << mstTime << "] \n";

}

void Log::logError(string log, bool emailFlag = false, bool timestampFlag = false) {
	Time time;

	string gmtTime;
	string unixTime;

	if (timestampFlag) {
		gmtTime = time.getGMTTime();
		unixTime = time.getUnixTime();
	}

	std::ofstream file("error_log.txt", std::ios_base::app);

	if (timestampFlag) {
		file << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << "] ";
	}

	file << log << "\n";

	if (emailFlag) {
		email("ERROR", log);
	}

	std::cout << "ERROR: " << log << std::endl;
}
