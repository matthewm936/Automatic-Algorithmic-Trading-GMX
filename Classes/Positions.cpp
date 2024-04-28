#ifndef Positions_cpp
#define Positions_cpp

#include <unordered_map>
#include <string>
#include <ctime>
#include <string>

using std::string;

struct Position {
	string tokenName;
	string timeFrame;
	string positionDirection = "";

	double entryPrice;

	double takeProfit;
	double stopLoss;

	double sizeUSD;
	int leverage;

	time_t entryTime;

	double profitLoss;
};

class Positions {
private:
	std::unordered_map<string, Position> positions;

	double totalExposureUSD;

	int MAX_POSITIONS = 5;

public:
	Positions() {}

	Position& operator[](const string& key) {
		return positions[key];
	}

	std::unordered_map<string, Position>::iterator find(const string& key) {
		return positions.find(key);
	}

	bool exists(const string& key) {
		return positions.find(key) != positions.end();
	}

	std::unordered_map<string, Position>::iterator end() {
		return positions.end();
	}

	void addPosition(const string& tokenName, const string& timeFrame, string direction, double entryPrice, double takeProfit, double stopLoss, double sizeUSD, int leverage) {
		string positionId = tokenName + "_" + timeFrame;  // Concatenate tokenName and timeFrame

		if (positions.find(positionId) != positions.end()) {
			Log::log("Position already exists, attempting to " + direction + " " + tokenName + " " + timeFrame);
			return;
		}

		positions.emplace(positionId, Position{
			.tokenName = tokenName,
			.timeFrame = timeFrame,
			.positionDirection = direction,
			.entryPrice = entryPrice,
			.takeProfit = takeProfit,
			.stopLoss = stopLoss,
			.sizeUSD = sizeUSD,
			.leverage = leverage,
			.entryTime = time(NULL)
		});

		totalExposureUSD += sizeUSD;
	}

	void removePosition(const string& tokenName) {
		positions.erase(tokenName);
	}

	size_t size() const {
		return positions.size();
	}

	Position& getPosition(const string& tokenName) {
		return positions[tokenName];
	}

	std::string toStringPositions(Candlesticks& candlesticks) const {
		std::string log;
		double totalProfit = 0.0;

		log += "Total Exposure USD: " + std::to_string(totalExposureUSD) + "\n";
		log += "Total Positions: " + std::to_string(positions.size()) + "\n";

		for (const auto& [positionId, position] : positions) {
			if (position.tokenName == candlesticks.getTokenName() && position.timeFrame == candlesticks.getTimeFrame()) { 
				double profitPercent = (position.positionDirection == "Long") ? 
									   (candlesticks.getCurrentPrice() - position.entryPrice) / position.entryPrice :
									   (position.entryPrice - candlesticks.getCurrentPrice()) / position.entryPrice;

				totalProfit += profitPercent;

				log += positionId + " Token Name: " + position.tokenName + "\n";
				log += positionId + " Position Direction: " + position.positionDirection + "\n";
				log += positionId + " Entry Price: " + std::to_string(position.entryPrice) + "\n";
				log += positionId + " Current Price: " + std::to_string(candlesticks.getCurrentPrice()) + "\n";
				log += positionId + " Profit%: " + std::to_string(profitPercent) + "\n";
				log += positionId + " Distance to Stop Loss%: " + std::to_string((position.stopLoss - candlesticks.getCurrentPrice()) / candlesticks.getCurrentPrice()) + "\n";
				log += positionId + " Distance to Take Profit%: " + std::to_string((position.takeProfit - candlesticks.getCurrentPrice()) / candlesticks.getCurrentPrice()) + "\n";
			}
		}

		log += "Total Profit%: " + std::to_string(totalProfit) + "\n";

		return log;
	}
};

#endif