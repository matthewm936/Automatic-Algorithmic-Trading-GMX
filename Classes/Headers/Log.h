#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> 
#include <ctime>
#include <filesystem> 
#include "Time.h" 

#include "Positions.h"
#include "TradingPair.h"
#include "TradingPairs.h"

class Log {
private:
	static std::string baseFilename;
	static std::string currentFilename;

	static void checkLogFile();

	static void email(std::string message);

public:
	static void clearLogFile();

	static void logNoNewline(std::string log);

	static void log(std::string log);

	static void logAndEmail(std::string log);

	static void LogWithTimestamp(std::string log);

	static void logError(std::string log);

	static void logPositions(const Positions& pos);

	static void logPair(const TradingPair& pair);

	static void logPairs(const TradingPairs& pairs);
};

#endif // LOG_H
