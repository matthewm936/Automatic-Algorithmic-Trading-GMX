#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> 
#include <ctime>
#include <string>

#include "Headers/Time.h"
#include "Headers/Positions.h"
#include "Headers/Log.h"
#include "Headers/TradingPair.h"
#include "Headers/TradingPairs.h"

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
		logfile << "Entry time: " << position.entryTime << std::endl;

		logfile << "-----------------" << std::endl;
	}

	logfile.close();
}

int getPrecision(double value) {
	if (value >= 1.0)
		return 2; // For values greater than or equal to 1, use 2 decimal places
	else {
		int precision = 0;
		double temp = value;
		while (temp < 1.0) {
			temp *= 10;
			precision++;
		}
		return precision; // For values less than 1, use a number of decimal places based on how small the value is
	}
}

void Log::logPair(const TradingPair& pair) {
	string logFilename = "Logs/pairs.txt";
	std::ofstream logfile(logFilename, std::ios::app);
	if (!logfile.is_open()) {
		Log::logError("Failed to open log file: " + logFilename);
		return;
	}

	int precision = getPrecision(pair.getCurrentPrice());

	logfile << "-----------------" << std::endl;
	logfile << "Pair Name: " << pair.pairName << std::endl;
	logfile << "Base Asset: " << pair.baseAsset << std::endl;
	logfile << "Quote Asset: " << pair.quoteAsset << std::endl;
	logfile << "Current Price: " << std::fixed << std::setprecision(precision) << pair.getCurrentPrice() << std::endl;
	logfile << "Ask Price: " << std::fixed << std::setprecision(2) << pair.askPrice << std::endl;
	logfile << "Bid Price: " << std::fixed << std::setprecision(2) << pair.bidPrice << std::endl;
	logfile << "Ask Quantity: " << std::fixed << std::setprecision(2) << pair.askQty << std::endl;
	logfile << "Bid Quantity: " << std::fixed << std::setprecision(2) << pair.bidQty << std::endl;
	logfile << "Volume: " << std::fixed << std::setprecision(2) << pair.volume << std::endl;
	logfile << "Quote Volume: " << std::fixed << std::setprecision(2) << pair.quoteVolume << std::endl;

	if (!logfile) {
		Log::logError("Failed to write to log file: " + logFilename);
		return;
	}
}

void Log::logPairs(const TradingPairs& pairs) {
	string logFilename = "Logs/pairs.txt";
	std::ofstream logfile(logFilename, std::ios::app);
	if (!logfile.is_open()) {
		Log::logError("Failed to open log file: " + logFilename);
		return;
	}

	logfile << "-----------------" << std::endl;
	logfile << "Pairs Details:" << std::endl;
	logfile << "total pairs: " << pairs.size() << std::endl;
	logfile << "total quote volume: " << std::endl;
	logfile << "USDT:" << std::fixed << std::setprecision(2) << pairs.quoteVolumeUSDT << std::endl;
	logfile << "USDC:" << std::fixed << std::setprecision(2) << pairs.quoteVolumeUSDC << std::endl;
	logfile << "BTC:" << std::fixed << std::setprecision(2) << pairs.quoteVolumeBTC << std::endl;
	logfile << "ETH:" << std::fixed << std::setprecision(2) << pairs.quoteVolumeETH << std::endl;
	logfile << "TUSD:" << std::fixed << std::setprecision(2) << pairs.quoteVolumeTUSD << std::endl;
	logfile << "-----------------" << std::endl;

	for(auto& pair : pairs.pairs) {
		logfile << "-----------------" << std::endl;
		logfile << "Pair Name: " << pair.first << std::endl;
		logfile << "Base Asset: " << pair.second.baseAsset << std::endl;
		logfile << "Quote Asset: " << pair.second.quoteAsset << std::endl;
		logfile << "Current Price: " << std::fixed << std::setprecision(2) << pair.second.getCurrentPrice() << std::endl;
		logfile << "Ask Price: " << std::fixed << std::setprecision(2) << pair.second.askPrice << std::endl;
		logfile << "Bid Price: " << std::fixed << std::setprecision(2) << pair.second.bidPrice << std::endl;
		logfile << "Ask Quantity: " << std::fixed << std::setprecision(2) << pair.second.askQty << std::endl;
		logfile << "Bid Quantity: " << std::fixed << std::setprecision(2) << pair.second.bidQty << std::endl;
		logfile << "Volume: " << std::fixed << std::setprecision(2) << pair.second.volume << std::endl;
		logfile << "Quote Volume: " << std::fixed << std::setprecision(2) << pair.second.quoteVolume << std::endl;
	}

	logfile.close();
}

string Log::baseFilename = "log";
string Log::currentFilename = "";
