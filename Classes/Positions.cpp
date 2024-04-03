#ifndef Positions_cpp
#define Positions_cpp

#include <unordered_map>
#include <string>
#include <ctime>
#include <string>

struct Position {
	std::string pairName;
	double entryPrice;
	double currentPrice;
	double profitLoss;

	time_t entryTime;
	time_t exitTime;

	double takeProfit;
	double stopLoss;

	time_t exitTimeCondition;
};

class Positions {
private:
	std::unordered_map<std::string, Position> positions;

public:
	Positions() {}

	Position& operator[](const std::string& key) {
		return positions[key];
	}

	std::unordered_map<std::string, Position>::iterator find(const std::string& key) {
		return positions.find(key);
	}

	bool exists(const std::string& key) {
		return positions.find(key) != positions.end();
	}

	std::unordered_map<std::string, Position>::iterator end() {
		return positions.end();
	}

	void addPosition(const std::string& pairName, double entryPrice, double takeProfit = -1, double stopLoss = -1, time_t exitTimeCondition = -1) {
		Position position;

		position.pairName = pairName;
		position.entryPrice = entryPrice;

		position.takeProfit = takeProfit;
		position.stopLoss = stopLoss;

		position.exitTimeCondition = exitTimeCondition;

		position.entryTime = time(0);
		positions[pairName] = position;
	}

	void removePosition(const std::string& pairName) {
		positions.erase(pairName);
	}

	size_t size() const {
		return positions.size();
	}

	Position& getPosition(const std::string& pairName) {
		return positions[pairName];
	}
};

#endif