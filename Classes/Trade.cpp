#ifndef Trade_cpp
#define Trade_cpp

class Trade {
private:
	bool strategyConsistentBullish(Candlesticks& candlesticks) {
		candlesticks.calculateCandleStatistics(1, 4);

		int bullish = 0;
		if(candlesticks.greenCandlePercent == 1 ) {
			bullish++;
		} 
		if(candlesticks.higherHighsPercent == 1) {
			bullish++;
		} 
		if(candlesticks.higherLowsPercent == 1) {
			bullish++;
		} 
		if(candlesticks.higherOpensPercent == 1) {
			bullish++;
		} 
		if(candlesticks.higherClosesPercent == 1) {
			bullish++;
		} 
		if(bullish > 4) {
			return true;
		}
		return false;
	}

	bool strategyConsistentBearish(Candlesticks& candlesticks) {
		candlesticks.calculateCandleStatistics(1, 4);

		int bearish = 0;
		if(candlesticks.redCandlePercent == 1 ) {
			bearish++;
		} 
		if(candlesticks.lowerLowsPercent == 1) {
			bearish++;
		} 
		if(candlesticks.lowerHighsPercent == 1) {
			bearish++;
		} 
		if(candlesticks.lowerOpensPercent == 1) {
			bearish++;
		} 
		if(candlesticks.lowerClosesPercent == 1) {
			bearish++;
		} 
		if(bearish > 4) {
			return true;
		}
		return false;
	}

	double calculateStopProfit(Candlesticks& candlesticks) {
		if(candlesticks.getTimeFrame() == "1h") {
			return 0.02;
		} else if(candlesticks.getTimeFrame() == "4h") {
			return 0.04;
		} else if(candlesticks.getTimeFrame() == "1d") {
			return 0.08;
		} else {
			return -1;
		}
	}

	void buyLong(Candlesticks& candlesticks) {
		//actually long

		double sizeUSD = -1;
		int leverage = -1;

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = candlesticks.entryPrice * (1 - stopProfit);
		double takeProfit = candlesticks.entryPrice * (1 + stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "long", candlesticks.entryPrice, candlesticks.takeProfit, candlesticks.stopLoss, sizeUSD, leverage);
	}

	void sellShort(Candlesticks& candlesticks) {
		//actually short

		double sizeUSD = -1;
		int leverage = -1;

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = candlesticks.entryPrice * (1 + stopProfit);
		double takeProfit = candlesticks.entryPrice * (1 - stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "short", candlesticks.entryPrice, candlesticks.takeProfit, candlesticks.stopLoss, sizeUSD, leverage);
	}

public:
	Positions positions;

	Trade(Positions& positions) : positions(positions) {}

	void trade(Candlesticks& candlesticks) {
		if(positions.exists(candlesticks.getTokenName() + "_" + candlesticks.getTimeFrame())) {
			return;
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