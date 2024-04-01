#ifndef POSITIONS_H
#define POSITIONS_H

#include <unordered_map>
#include <string>
#include <ctime>

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
	public:
		std::unordered_map<std::string, Position> positions;

		Positions();

		Position& operator[](const std::string& key);

		std::unordered_map<std::string, Position>::iterator find(const std::string& key);

		bool exists(const std::string& key);

		std::unordered_map<std::string, Position>::iterator end();

		void addPosition(const std::string& pairName, double entryPrice, double takeProfit, double stopLoss, time_t exitTimeCondition);

		void removePosition(const std::string& pairName);

		size_t size() const;

		Position& getPosition(const std::string& pairName);
};

#endif