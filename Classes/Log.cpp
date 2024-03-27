#ifndef Log_CPP
#define Log_CPP

#include <fstream>
#include <sstream>

#include "Time.cpp"

using namespace std;

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

public:
	static void logNoNewline(string log) {
		ofstream file(currentFilename, ios_base::app);
		file << log;
	
	}
	static void log(string log) {
		ofstream file(currentFilename, ios_base::app);
		file << log << "\n";
	}

	static void tradeLog(string log) {
		ofstream file("trades.txt", ios_base::app);
		file << log << "\n";
	}

	static void LogWithTimestamp(string log) {
		Time time;
		ofstream file(currentFilename, ios_base::app);
		string gmtTime = time.getGMTTime();
		string unixTime = time.getUnixTime();

		file << log << "\n"
			<< "| GMT Time: " << gmtTime << " | Unix Time: " << unixTime << " |\n";
	}

	static void logLine() {
		ofstream file(currentFilename, ios_base::app);
		file << "-------------------------------------------------------\n";
	}

	static void email(string message) {
		system(("python3 /home/johnsmith/Trading/Algorithmic-Trading/Notifications/pi-email-message.py " + message).c_str());
	}
	// static void logSell(Position position, string condition) {
	// 	double percentProfitLoss = (pair.getCurrentPrice() - positions[pair.pairName].entryPrice) / positions[pair.pairName].entryPrice;

	// 	string log = "Pair " + pair.pairName + " " + condition + " " + std::to_string(percentProfitLoss) + ". Position: Entry Price - " + std::to_string(positions[pair.pairName].entryPrice) + ", Exit Time Condition - " + std::to_string(positions[pair.pairName].exitTimeCondition);
	// }
};

string Log::baseFilename = "log";
string Log::currentFilename = "";
#endif