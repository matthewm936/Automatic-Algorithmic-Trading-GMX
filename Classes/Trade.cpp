#ifndef Trade_cpp
#define Trade_cpp

class Trade {
private:
    static constexpr double DEFUALT_USD_SIZE = 80;
    static constexpr double DEFUALT_LEVERAGE = 4;
    static constexpr double DEFUALT_STOP_PROFIT_5M = 0.02;
    static constexpr double DEFUALT_STOP_PROFIT_1H = 0.02;
    static constexpr double DEFUALT_STOP_PROFIT_4H = 0.02;
    static constexpr double DEFUALT_STOP_PROFIT_1D = 0.02;

	bool strategyConsistentBullish(Candlesticks& candlesticks) {
		candlesticks.calculateCandleStatistics(1, 4);
	
		int bullish = 0;
		bullish += (candlesticks.greenCandlePercent == 1);
		bullish += (candlesticks.higherHighsPercent == 1);
		bullish += (candlesticks.higherLowsPercent == 1);
		bullish += (candlesticks.higherOpensPercent == 1);
		bullish += (candlesticks.higherClosesPercent == 1);
	
		return (bullish >= 4);
	}
	
	bool strategyConsistentBearish(Candlesticks& candlesticks) {
		candlesticks.calculateCandleStatistics(0, 3);
	
		int bearish = 0;
		bearish += (candlesticks.redCandlePercent == 1);
		bearish += (candlesticks.lowerLowsPercent == 1);
		bearish += (candlesticks.lowerHighsPercent == 1);
		bearish += (candlesticks.lowerOpensPercent == 1);
		bearish += (candlesticks.lowerClosesPercent == 1);
	
		return (bearish >= 4);
	}

	double calculateStopProfit(const Candlesticks& candlesticks) {
		static const std::map<std::string, double> stopProfits = {
			{"5m", DEFUALT_STOP_PROFIT_5M},
			{"1h", DEFUALT_STOP_PROFIT_1H},
			{"4h", DEFUALT_STOP_PROFIT_4H},
			{"1d", DEFUALT_STOP_PROFIT_1D}
		};

		auto it = stopProfits.find(candlesticks.getTimeFrame());
		if (it != stopProfits.end()) {
			return it->second;
		} else {
			Log::logError("Invalid time frame");
			return 0;
		}
	}

	void buyLong(const Candlesticks& candlesticks) { // FIXME: both the long and short trades here might fail but the position tracker might still be on
		string longOrder = "python3 gmx_pythonsdk-0.0.4 create_increase_order.py " + candlesticks.getTokenName() + " true " + to_string(DEFUALT_USD_SIZE) + " " + to_string(DEFUALT_LEVERAGE);
		runCommand(longOrder.c_str());

		double currentPrice = candlesticks.getCurrentPrice();

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = currentPrice * (1 - stopProfit);
		double takeProfit = currentPrice * (1 + stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "long", currentPrice, takeProfit, stopLoss, DEFUALT_USD_SIZE, DEFUALT_LEVERAGE);
	}

	void sellShort(const Candlesticks& candlesticks) {
		string shortOrder = "python3 gmx_pythonsdk-0.0.4 create_increase_order.py " + candlesticks.getTokenName() + " false " + to_string(DEFUALT_USD_SIZE) + " " + to_string(DEFUALT_LEVERAGE);
		runCommand(shortOrder.c_str());
	
		double currentPrice = candlesticks.getCurrentPrice();

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = currentPrice * (1 + stopProfit);
		double takeProfit = currentPrice * (1 - stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "short", currentPrice, takeProfit, stopLoss, DEFUALT_USD_SIZE, DEFUALT_LEVERAGE);
	}

public:
    Positions& positions;

	Trade(Positions& positions) : positions(positions) {}

	void trade(Candlesticks& candlesticks) {
		if(candlesticks.getTimeFrame() == "1h") {
			return;
		}
		string positionKey = candlesticks.getTokenName() + "_" + candlesticks.getTimeFrame();
	
		if(positions.exists(positionKey)) {
			Position& currentPosition = positions.getPosition(positionKey);
			string positionDirection = currentPosition.positionDirection;
			double currentPrice = candlesticks.getCurrentPrice();
			string positionDuration = positions.getPositionDuration(positionKey);
	
			if(positionDirection == "long") {
				if(currentPrice >= currentPosition.takeProfit) {
					positions.removePosition(positionKey);
					Log::logTrade("Take Profit Long " + positionKey + " " + positionDuration);
				}
				if(currentPrice <= currentPosition.stopLoss) {
					positions.removePosition(positionKey);
					Log::logTrade("Stop Loss Long " + positionKey + " " + positionDuration);
				}
			}
			else if(positionDirection == "short") {
				if(currentPrice <= currentPosition.takeProfit) {
					positions.removePosition(positionKey);
					Log::logTrade("Take Profit Short " + positionKey + " " + positionDuration);
				}
				if(currentPrice >= currentPosition.stopLoss) {
					positions.removePosition(positionKey);
					Log::logTrade("Stop Loss Short " + positionKey + " " + positionDuration);
				}
			}
			return;
		}
		if(strategyConsistentBullish(candlesticks)) {
			buyLong(candlesticks);
			Log::log("Long " + positionKey);	
		}
		if(strategyConsistentBearish(candlesticks)) {
			sellShort(candlesticks);
			Log::log("Short " + positionKey);	
		}
	}
};

#endif