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
        time_t hours = duration / hour;
        time_t minutes = (duration % hour) / minute;
        ss << hours << " hours";
        if (minutes > 0)
            ss << " and " << minutes << " minutes";
    } else {
        time_t days = duration / day;
        time_t hours = (duration % day) / hour;
        time_t minutes = ((duration % day) % hour) / minute;
        ss << days << " days";
        if (hours > 0)
            ss << " and " << hours << " hours";
        if (minutes > 0)
            ss << " and " << minutes << " minutes";
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

public:
	int longStopLoss = 0;
	int longTakeProfit = 0;

	int shortStopLoss = 0;
	int shortTakeProfit = 0;

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
		string positionId = tokenName + "_" + timeFrame;

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

	void removePosition(const string& positionId) {
		auto it = positions.find(positionId);
		if (it != positions.end()) {
			totalExposureUSD -= it->second.sizeUSD;
			positions.erase(it);
		}
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

	string getPositionDuration(string positionId) {
		auto it = positions.find(positionId);
		if (it != positions.end()) {
			Position& position = it->second;
			position.positionDuration = time(NULL) - position.entryTime;
			return formatDuration(position.positionDuration);
		}
		return "Position not found";
	}

	std::string toStringPositions() {
		std::string log;
		double totalProfit = 0.0;

		log += "Total Exposure USD: " + to_string(totalExposureUSD) + "\n";
		log += "Total Positions: " + to_string(positions.size()) + "\n";
		log += "total long position take profits/stop loss: " + to_string(longTakeProfit) + "/" + to_string(longStopLoss) +"\n";
		log += "total short position take profits/stop loss: " + to_string(shortTakeProfit) + "/" + to_string(shortStopLoss) +"\n";

		for (auto& [positionId, position] : positions) {
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

			log += positionId + " position direction: " + position.positionDirection + "\n";
			log += positionId + " duration " + getPositionDuration(positionId) + "\n";
			log += positionId + " Entry Price: " + to_string(position.entryPrice) + "\n";
			log += positionId + " Current Price: " + to_string(currentPrice) + "\n";
			log += positionId + " Profit %: " + to_string(profitPercent) + "\n";
			log += positionId + " Highest Profit%: " + to_string(position.highestProfit) + "\n";
			log += positionId + " Highest Loss%: " + to_string(position.highestLoss) + "\n";
			log += positionId + " Distance to Stop Loss%: " + to_string((position.stopLoss - currentPrice) / currentPrice) + "\n";
			log += positionId + " Distance to Take Profit%: " + to_string((position.takeProfit - currentPrice) / currentPrice) + "\n";
		}

		log += "Total Profit%: " + to_string(totalProfit) + "\n";

		return log;
	}
};

#endif