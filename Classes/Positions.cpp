#ifndef Positions_cpp
#define Positions_cpp

#include <unordered_map>
#include <string>
#include <ctime>
#include <string>

using std::string;

std::string formatDuration(time_t duration) {
    const time_t minute = 60;
    const time_t hour = 60 * minute;
    const time_t day = 24 * hour;

    std::stringstream ss;
    if (duration < hour) {
        ss << duration / minute << " minutes";
    } else if (duration < day) {
        ss << duration / hour << " hours";
    } else {
        ss << duration / day << " days";
    }
    return ss.str();
}

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

	time_t positionDuration;

	double profitLoss;

	double highestProfit;
	double highestLoss;
};

class Positions {
private:
	std::unordered_map<string, Position> positions;
	std::unordered_map<string, Token>& GMX_tokens;

	double totalExposureUSD;

	int MAX_POSITIONS = 5;

public:
    Positions(unordered_map<string, Token>& GMX_tokens) : GMX_tokens(GMX_tokens) {}

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

	double getCurrentPriceForToken(const string& tokenName, const string& timeFrame) {
		return GMX_tokens[tokenName].getCandlesticks(timeFrame).getCurrentPrice();
	}

	std::string toStringPositions() {
		std::string log;
		double totalProfit = 0.0;

		log += "Total Exposure USD: " + std::to_string(totalExposureUSD) + "\n";
		log += "Total Positions: " + std::to_string(positions.size()) + "\n";

		for (const auto& [positionId, position] : positions) {
			double currentPrice = getCurrentPriceForToken(position.tokenName, position.timeFrame);

			double profitPercent = (position.positionDirection == "Long") ? 
									(currentPrice - position.entryPrice) / position.entryPrice :
									(position.entryPrice - currentPrice) / position.entryPrice;

			totalProfit += profitPercent;

			if(profitPercent > position.highestProfit) {
				position.highestProfit = profitPercent;
			}
			if(profitPercent < position.highestLoss) {
				position.highestLoss = profitPercent;
			}
			position.positionDuration = time(NULL) - position.entryTime;

			log += positionId + " position direction: " + position.positionDirection + "\n";
			log += positionId + " duration " + formatDuration(position.positionDuration) + "\n";
			log += positionId + " Entry Price: " + std::to_string(position.entryPrice) + "\n";
			log += positionId + " Current Price: " + std::to_string(currentPrice) + "\n";
			log += positionId + " Profit %: " + std::to_string(profitPercent) + "\n";
			log += positionId + " Highest Profit%: " + std::to_string(position.highestProfit) + "\n";
			log += positionId + " Highest Loss%: " + std::to_string(position.highestLoss) + "\n";
			log += positionId + " Distance to Stop Loss%: " + std::to_string((position.stopLoss - currentPrice) / currentPrice) + "\n";
			log += positionId + " Distance to Take Profit%: " + std::to_string((position.takeProfit - currentPrice) / currentPrice) + "\n";
		}

		log += "Total Profit%: " + std::to_string(totalProfit) + "\n";

		return log;
	}
};

#endif