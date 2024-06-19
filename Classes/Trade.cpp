#ifndef Trade_cpp
#define Trade_cpp

class Trade {
private:
    static constexpr double DEFUALT_USD_SIZE = 50;
    static constexpr double DEFUALT_LEVERAGE = 1;
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

		bullish += (candlesticks.getCandles()[1].WickRatioIndex >= 0.6);
		bullish += (candlesticks.getCandles()[2].WickRatioIndex >= 0.6);
		bullish += (candlesticks.getCandles()[3].WickRatioIndex >= 0.6);
		bullish += (candlesticks.getCandles()[4].WickRatioIndex >= 0.6);

		if(bullish >= 6) {
			string log = "strategyConsistentBullish; bullish value: " + to_string(bullish) + " on " + candlesticks.getTokenName() + " " + candlesticks.getTimeFrame() + "\n" 
							+ "WickRatioIndex[1]: " + to_string(candlesticks.getCandles()[1].WickRatioIndex) + ", "
							+ "WickRatioIndex[2]: " + to_string(candlesticks.getCandles()[2].WickRatioIndex) + ", "
							+ "WickRatioIndex[3]: " + to_string(candlesticks.getCandles()[3].WickRatioIndex) + ", "
							+ "WickRatioIndex[4]: " + to_string(candlesticks.getCandles()[4].WickRatioIndex);
			Log::logTradeStrat(log);
		}

		return (bullish >= 8);
	}
	
	bool strategyConsistentBearish(Candlesticks& candlesticks) {
		candlesticks.calculateCandleStatistics(1, 3);

		int bearish = 0;
		bearish += (candlesticks.redCandlePercent == 1);
		bearish += (candlesticks.lowerLowsPercent == 1);
		bearish += (candlesticks.lowerHighsPercent == 1);
		bearish += (candlesticks.lowerOpensPercent == 1);
		bearish += (candlesticks.lowerClosesPercent == 1);

		bearish += (candlesticks.getCandles()[1].WickRatioIndex <= -0.6);
		bearish += (candlesticks.getCandles()[2].WickRatioIndex <= -0.6);

		if(bearish >= 5) {
			string log = "strategyConsistentBearish; bearish value: " + to_string(bearish) + " on " + candlesticks.getTokenName() + " " + candlesticks.getTimeFrame() + "\n" 
							+ "WickRatioIndex[1]: " + to_string(candlesticks.getCandles()[1].WickRatioIndex) + ", "
							+ "WickRatioIndex[2]: " + to_string(candlesticks.getCandles()[2].WickRatioIndex);

			Log::logTradeStrat(log);
		}

		return (bearish >= 6);
	}

	double calculateStopProfit(const Candlesticks& candlesticks) {
		static const std::map<string, double> stopProfits = {
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

	void openLong(const Candlesticks& candlesticks) { // FIXME: both the long and short trades here might fail but the position tracker might still be on
		string longOrder = "python3 gmx_python_sdk/create_increase_order.py " + candlesticks.getTokenName() + " true " + to_string(DEFUALT_USD_SIZE) + " " + to_string(DEFUALT_LEVERAGE);
		runCommand(longOrder.c_str());

		Log::logAndEmail("opened long");

		double currentPrice = candlesticks.getCurrentPrice();

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = currentPrice * (1 - stopProfit);
		double takeProfit = currentPrice * (1 + stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "long", currentPrice, takeProfit, stopLoss, DEFUALT_USD_SIZE, DEFUALT_LEVERAGE);
	}

	void openShort(const Candlesticks& candlesticks) {
		string shortOrder = "python3 gmx_python_sdk/create_increase_order.py " + candlesticks.getTokenName() + " false " + to_string(DEFUALT_USD_SIZE) + " " + to_string(DEFUALT_LEVERAGE);
		runCommand(shortOrder.c_str());

		Log::logAndEmail("opened short");

		double currentPrice = candlesticks.getCurrentPrice();

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = currentPrice * (1 + stopProfit);
		double takeProfit = currentPrice * (1 - stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "short", currentPrice, takeProfit, stopLoss, DEFUALT_USD_SIZE, DEFUALT_LEVERAGE);
	}

	void closePosition(string symbol, string isLong) {
		string closeOrder = "python3 gmx_python_sdk/create_decrease_order_with_known_positions.py " + symbol + " " + isLong;
		runCommand(closeOrder.c_str());
	}

public:
    Positions& positions;

	Trade(Positions& positions) : positions(positions) {}

	void trade(Candlesticks& candlesticks) {
		string positionKey = candlesticks.getTokenName() + "_" + candlesticks.getTimeFrame();
	
		if(positions.exists(positionKey)) {
			Position& currentPosition = positions.getPosition(positionKey);
			string positionDirection = currentPosition.positionDirection;
			double currentPrice = candlesticks.getCurrentPrice();
			string positionDuration = positions.getPositionDuration(positionKey);
	
			if(positionDirection == "long") {
				if(currentPrice >= currentPosition.takeProfit) {
					closePosition(candlesticks.getTokenName(), "true");
					positions.removePosition(positionKey);
					Log::logTrade("Take Profit Long " + positionKey + " " + positionDuration);

					positions.longTakeProfit++;
				}
				if(currentPrice <= currentPosition.stopLoss) {
					closePosition(candlesticks.getTokenName(), "true");
					positions.removePosition(positionKey);
					Log::logTrade("Stop Loss Long " + positionKey + " " + positionDuration);

					positions.longStopLoss++;
				}
			}
			else if(positionDirection == "short") {
				if(currentPrice <= currentPosition.takeProfit) {
					closePosition(candlesticks.getTokenName(), "false");
					positions.removePosition(positionKey);
					Log::logTrade("Take Profit Short " + positionKey + " " + positionDuration);

					positions.shortTakeProfit++;
				}
				if(currentPrice >= currentPosition.stopLoss) {
					closePosition(candlesticks.getTokenName(), "false");
					positions.removePosition(positionKey);
					Log::logTrade("Stop Loss Short " + positionKey + " " + positionDuration);

					positions.shortStopLoss++;
				}
			}
			return;
		}

		if(candlesticks.getTimeFrame() != "1h") { 
			if(strategyConsistentBullish(candlesticks)) {
				openLong(candlesticks);
				string log = "1h long strat consistent bullish " + positionKey;
				Log::logTrade(log);
				Log::email("long", log);
			}
		}

		if(candlesticks.getTimeFrame() == "1h") {
			if(strategyConsistentBearish(candlesticks)) { //pretty sure the logic for wick candle isnt bounded by [-1, 1]
				openShort(candlesticks);
				string log = "1h short strat consistent bearish " + positionKey;
				Log::logTrade(log);
				Log::email("short", log);
			}
		}
	}
};

#endif