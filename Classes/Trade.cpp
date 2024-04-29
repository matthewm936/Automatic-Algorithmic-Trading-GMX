#ifndef Trade_cpp
#define Trade_cpp

class Trade {
private:
    static constexpr double DEFUALT_USD_SIZE = 100;
    static constexpr double DEFUALT_LEVERAGE = 10;
    static constexpr double DEFUALT_STOP_PROFIT_5M = 0.02;
    static constexpr double DEFUALT_STOP_PROFIT_1H = 0.02;
    static constexpr double DEFUALT_STOP_PROFIT_4H = 0.02;
    static constexpr double DEFUALT_STOP_PROFIT_1D = 0.02;

	bool strategyConsistentBullish(Candlesticks candlesticks) {
		candlesticks.calculateCandleStatistics(1, 4);
	
		int bullish = 0;
		bullish += (candlesticks.greenCandlePercent == 1);
		bullish += (candlesticks.higherHighsPercent == 1);
		bullish += (candlesticks.higherLowsPercent == 1);
		bullish += (candlesticks.higherOpensPercent == 1);
		bullish += (candlesticks.higherClosesPercent == 1);
	
		return (bullish >= 4);
	}
	
	bool strategyConsistentBearish(Candlesticks candlesticks) {
		candlesticks.calculateCandleStatistics(0, 3);
	
		int bearish = 0;
		bearish += (candlesticks.redCandlePercent == 1);
		bearish += (candlesticks.lowerLowsPercent == 1);
		bearish += (candlesticks.lowerHighsPercent == 1);
		bearish += (candlesticks.lowerOpensPercent == 1);
		bearish += (candlesticks.lowerClosesPercent == 1);
	
		return (bearish >= 4);
	}

	double calculateStopProfit(const Candlesticks candlesticks) {
		if(candlesticks.getTimeFrame() == "5m") {
			return DEFUALT_STOP_PROFIT_5M;
		} else if(candlesticks.getTimeFrame() == "1h") {
		return DEFUALT_STOP_PROFIT_1H;
		} else if(candlesticks.getTimeFrame() == "4h") {
			return DEFUALT_STOP_PROFIT_4H;
		} else if(candlesticks.getTimeFrame() == "1d") {
			return DEFUALT_STOP_PROFIT_1D;
		} else {
			Log::logError("Invalid time frame");
			return 0;
		}
	}

	void buyLong(const Candlesticks candlesticks) {
		//actually long
		double currentPrice = candlesticks.getCurrentPrice();

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = currentPrice * (1 - stopProfit);
		double takeProfit = currentPrice * (1 + stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "long", currentPrice, takeProfit, stopLoss, DEFUALT_USD_SIZE, DEFUALT_LEVERAGE);
	}

	void sellShort(const Candlesticks candlesticks) {
		//actually short
		double currentPrice = candlesticks.getCurrentPrice();

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = currentPrice * (1 + stopProfit);
		double takeProfit = currentPrice * (1 - stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "short", currentPrice, takeProfit, stopLoss, DEFUALT_USD_SIZE, DEFUALT_LEVERAGE);
	}

public:
    Positions& positions;

	Trade(Positions& positions) : positions(positions) {}

	void trade(const Candlesticks candlesticks) {
		if(positions.exists(candlesticks.getTokenName() + "_" + candlesticks.getTimeFrame())) {
			return; // this logic is repeated twice, once here and again is positions to not enter into a postiion again, not sure which I want
		}
		if(strategyConsistentBullish(candlesticks)) {
			buyLong(candlesticks);
			
			Log::log("Long " + candlesticks.getTokenName());	
		}
		if(strategyConsistentBearish(candlesticks)) {
			sellShort(candlesticks);
			
			Log::log("Short " + candlesticks.getTokenName());	
		}
	}
};

#endif