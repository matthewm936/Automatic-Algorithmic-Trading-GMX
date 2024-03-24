#ifndef Log_CPP
#define Log_CPP

#include <fstream>
#include <sstream>

#include "Time.cpp"

using namespace std;

class Log {
private:
	static string filename;

public:
	static void logNoNewline(string log) {
		ofstream file(filename, ios_base::app);
		file << log;
	
	}
	static void log(string log) {
		ofstream file(filename, ios_base::app);
		file << log << "\n";
	}

	static void tradeLog(string log) {
		ofstream file("trades.txt", ios_base::app);
		file << log << "\n";
	}

	static void LogWithTimestamp(string log) {
		Time time;
		ofstream file(filename, ios_base::app);
		string gmtTime = time.getGMTTime();
		string unixTime = time.getUnixTime();

		file << log << "\n"
			<< "| GMT Time: " << gmtTime << " | Unix Time: " << unixTime << " |\n";
	}

	static void logLine() {
		ofstream file(filename, ios_base::app);
		file << "-------------------------------------------------------\n";
	}

	// static void logSell(Position position, string condition) {
	// 	double percentProfitLoss = (pair.getCurrentPrice() - positions[pair.pairName].entryPrice) / positions[pair.pairName].entryPrice;

	// 	string log = "Pair " + pair.pairName + " " + condition + " " + std::to_string(percentProfitLoss) + ". Position: Entry Price - " + std::to_string(positions[pair.pairName].entryPrice) + ", Exit Time Condition - " + std::to_string(positions[pair.pairName].exitTimeCondition);
	// }
};

string Log::filename = "log.txt";

#endif