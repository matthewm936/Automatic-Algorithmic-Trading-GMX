#include <unordered_map>
#include <string>
#include <ctime>
#include <string>

#include "Headers/Positions.h"

Positions::Positions() {}

Position& Positions::operator[](const std::string& key) {
	return positions[key];
}

std::unordered_map<std::string, Position>::iterator Positions::find(const std::string& key) {
	return positions.find(key);
}

bool Positions::exists(const std::string& key) {
	return positions.find(key) != positions.end();
}

std::unordered_map<std::string, Position>::iterator Positions::end() {
	return positions.end();
}

void Positions::addPosition(const std::string& pairName, double entryPrice, double takeProfit = -1, double stopLoss = -1, time_t exitTimeCondition = -1) {
	Position position;
	
	position.pairName = pairName;
	position.entryPrice = entryPrice;

	position.takeProfit = takeProfit;
	position.stopLoss = stopLoss;

	position.exitTimeCondition = exitTimeCondition;

	position.entryTime = time(0);
	positions[pairName] = position;
}

void Positions::removePosition(const std::string& pairName) {
	positions.erase(pairName);
}

size_t Positions::size() const {
	return positions.size();
}

Position& Positions::getPosition(const std::string& pairName) {
	return positions[pairName];
}
