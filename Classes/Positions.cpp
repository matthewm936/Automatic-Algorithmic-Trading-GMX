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

	void logPositions(Candlesticks& candlesticks) const {
		std::map<string, string> currentState;

		currentState["Total Exposure USD"] = std::to_string(totalExposureUSD);
		currentState["Total Positions"] = std::to_string(positions.size());

		for (const auto& [positionId, position] : positions) {
			if (position.tokenName == candlesticks.getTokenName() && position.timeFrame == candlesticks.getTimeFrame()) {
				string positionDirection = (position.entryPrice < candlesticks.getCurrentPrice()) ? "Long" : "Short";
				double profitPercent = (positionDirection == "Long") ? 
									   (candlesticks.getCurrentPrice() - position.entryPrice) / position.entryPrice :
									   (position.entryPrice - candlesticks.getCurrentPrice()) / position.entryPrice;
		
				std::map<string, string> currentState = {
					{positionId + " Token Name", position.tokenName},
					{positionId + " Position Direction", positionDirection},
					{positionId + " Entry Price", to_string(position.entryPrice)},
					{positionId + " Current Price", to_string(candlesticks.getCurrentPrice())},
					{positionId + " Profit%", to_string(profitPercent)},
					{positionId + " Distance to Stop Loss%", to_string((position.stopLoss - candlesticks.getCurrentPrice()) / candlesticks.getCurrentPrice())},
					{positionId + " Distance to Take Profit%", to_string((position.takeProfit - candlesticks.getCurrentPrice()) / candlesticks.getCurrentPrice())},
				};
		
				Log::logCurrentState(currentState, "positions.txt");
			}
		}

		Log::logCurrentState(currentState, "positions.txt");
	}
};

#endif