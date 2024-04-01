#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> 
#include <ctime>
#include <string>

#include "Headers/Time.h"
#include "Headers/Positions.h"
#include "Headers/Log.h"

using namespace std;

// All error logs get logged to an error file and automatically emailed

string baseFilename;
string currentFilename;

void Log::checkLogFile() {
	Time time;
	string currentDate = time.getGMTTime().substr(0, 10); 

	if (currentFilename != baseFilename + currentDate) {
		string oldFilename = currentFilename;
		currentFilename = baseFilename + currentDate;

		if (!oldFilename.empty()) {
			filesystem::remove(oldFilename);
		}
	}
}

void Log::email(string message) {
	string command = "python3 /home/johnsmith/Trading/Algorithmic-Trading/Notifications/pi-email-message.py \"" + message + "\"";
	system(command.c_str());
}

void Log::clearLogFile() {
	checkLogFile();
	ofstream file(currentFilename, ios_base::trunc);
	file.close();
}

void Log::logNoNewline(string log) {
	checkLogFile();
	ofstream file(currentFilename, ios_base::app);
	file << log;

}
void Log::log(string log) {
	checkLogFile();
	ofstream file(currentFilename, ios_base::app);
	file << log << "\n";
}

void Log::logAndEmail(string log) {
	checkLogFile();
	ofstream file(currentFilename, ios_base::app);
	file << log << "\n";

	email(log);
}

void Log::LogWithTimestamp(string log) {
	checkLogFile();
	Time time;
	ofstream file(currentFilename, ios_base::app);
	string gmtTime = time.getGMTTime();
	string unixTime = time.getUnixTime();

	file << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << "] " << log << "\n";
}

void Log::logError(string log) {
	Time time;
	string gmtTime = time.getGMTTime();
	string unixTime = time.getUnixTime();

	ofstream file("error_log.txt", ios_base::app);
	file << "[GMT Time: " << gmtTime << ", Unix Time: " << unixTime << "] " << log << "\n";

	email(log);
}

void Log::logPositions(const Positions& pos) {
	string logFilename = "Logs/positions.txt";
	std::ofstream logfile(logFilename, std::ios::app);
	if (!logfile.is_open()) {
		Log::logError("Failed to open log file: " + logFilename);
		return;
	}

	logfile << "Position Details:" << std::endl;
	logfile << "total positions: " << pos.size() << std::endl;
	logfile << "-----------------" << std::endl;
	for (const auto& pair : pos.positions) {
		const auto& position = pair.second;

		logfile << "Pair Name: " << position.pairName << std::endl;
		logfile << "Entry Price: " << std::fixed << std::setprecision(2) << position.entryPrice << std::endl;
		logfile << "Current Price: " << std::fixed << std::setprecision(2) << position.currentPrice << std::endl;
		logfile << "Profit/Loss: " << std::fixed << std::setprecision(2) << position.profitLoss << std::endl;

		logfile << "-----------------" << std::endl;
	}

	logfile.close();
	}


string Log::baseFilename = "log";
string Log::currentFilename = "";
