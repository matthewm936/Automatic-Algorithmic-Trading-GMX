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
	static void logError(std::string log, bool emailFlag, bool timestampFlag);
	static void logCurrentState(std::map<std::string, std::string> updatedState, std::string filename);
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

void Log::email(std::string header, std::string message) {
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

	file << log << "\n" << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << ", MST Time: " << mstTime << "] \n";
}

void Log::logError(std::string log, bool emailFlag = false, bool timestampFlag = false) {
	Time time;
	std::string gmtTime;
	std::string unixTime;

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
}

void Log::logCurrentState(std::map<std::string, std::string> updatedState, std::string filename) {
	checkLogFile();

	// Read current state from log file
	std::map<std::string, std::string> currentState;
	std::ifstream fileIn(filename);
	std::string line;
	while (std::getline(fileIn, line)) {
		size_t pos = line.find(": ");
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 2);
			currentState[key] = value;
		}
	}
	fileIn.close();

	// Update state
	for (const auto& [key, value] : updatedState) {
		currentState[key] = value;
	}

	// Write updated state to log file
	std::ofstream fileOut(filename, std::ios_base::trunc);
	for (const auto& [key, value] : currentState) {
		fileOut << key << ": " << value << "\n";
	}
	fileOut.close();
}