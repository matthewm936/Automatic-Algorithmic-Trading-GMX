#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> 
#include <ctime>
#include <string>
#include <chrono>
#include <filesystem>

#include "Time.cpp"

class Log {
private:
	static std::string baseFilename;
	static std::string currentFilename;

	static void checkLogFile();
	static void email(std::string message, std::string header);

public:
	static void clearLogFiles();
	static void logNoNewline(std::string log);
	static void log(std::string log);
	static void logAndEmail(std::string log);
	static void LogWithTimestamp(std::string log);
	static void logError(std::string log);
};

// Define static member variables
std::string Log::baseFilename = "log";
std::string Log::currentFilename = "";

// Define static member functions
void Log::checkLogFile() {
	Time time;
	std::string currentDate = time.getGMTTime().substr(0, 10); 

	if (currentFilename != baseFilename + currentDate) {
		std::string oldFilename = currentFilename;
		currentFilename = baseFilename + currentDate;

		if (!oldFilename.empty()) {
			std::filesystem::remove(oldFilename);
		}
	}
}

void Log::email(std::string message, std::string header) {
	std::string command = "python3 /home/johnsmith/Trading/Algorithmic-Trading/Notifications/pi-email-message.py \"" + header + "\" \"" + message + "\"";
	system(command.c_str());
}

void Log::clearLogFiles() {
	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::trunc);
	std::ofstream errorFile("error_log.txt", std::ios_base::trunc);
	std::ofstream positionsFile("Logs/positions.txt", std::ios_base::trunc);
	std::ofstream pairsFile("Logs/pairs.txt", std::ios_base::trunc);
	std::ofstream strategyFile("Logs/strategy.txt", std::ios_base::trunc);

	file.close();
	errorFile.close();
	positionsFile.close();
	pairsFile.close();
	strategyFile.close();
}

void Log::logNoNewline(std::string log) {
	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::app);
	file << log;
}

void Log::log(std::string log) {
	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::app);
	file << log << "\n";
}

void Log::logAndEmail(std::string log) {
	checkLogFile();
	std::ofstream file(currentFilename, std::ios_base::app);
	file << log << "\n";

	email("regular log", log);
}

void Log::LogWithTimestamp(std::string log) {
	checkLogFile();
	Time time;
	std::ofstream file(currentFilename, std::ios_base::app);
	std::string gmtTime = time.getGMTTime();
	std::string unixTime = time.getUnixTime();
	std::string mstTime = time.getMSTTime();

	file << "\n" << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << ", MST Time: " << mstTime << "] " << log;
}

void Log::logError(std::string log) {
	Time time;
	std::string gmtTime = time.getGMTTime();
	std::string unixTime = time.getUnixTime();

	std::ofstream file("error_log.txt", std::ios_base::app);
	file << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << "] " << log << "\n";

	email("ERROR", log);
}
