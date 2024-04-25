#ifndef Trade_cpp
#define Trade_cpp

class Trade {
private:
	bool strategyConsistentBullish(Candlesticks& candlesticks) {
		candlesticks.calculateCandleStatistics(1, 4);
	
		int bullish = 0;
		bullish += (candlesticks.greenCandlePercent == 1);
		bullish += (candlesticks.higherHighsPercent == 1);
		bullish += (candlesticks.higherLowsPercent == 1);
		bullish += (candlesticks.higherOpensPercent == 1);
		bullish += (candlesticks.higherClosesPercent == 1);
	
		return (bullish > 4);
	}
	
	bool strategyConsistentBearish(Candlesticks& candlesticks) {
		candlesticks.calculateCandleStatistics(1, 4);
	
		int bearish = 0;
		bearish += (candlesticks.redCandlePercent == 1);
		bearish += (candlesticks.lowerLowsPercent == 1);
		bearish += (candlesticks.lowerHighsPercent == 1);
		bearish += (candlesticks.lowerOpensPercent == 1);
		bearish += (candlesticks.lowerClosesPercent == 1);
	
		return (bearish > 4);
	}
	
	double calculateStopProfit(Candlesticks& candlesticks) {
		if(candlesticks.getTimeFrame() == "5m") return 0.01;
		else if(candlesticks.getTimeFrame() == "1h") return 0.02;
		else if(candlesticks.getTimeFrame() == "4h") return 0.04;
		else if(candlesticks.getTimeFrame() == "1d") return 0.08;
		else return -1;
	}

	double calculateSizeUSD() {
		return 100;
	}

	int calcualteLeverage() {
		if(candlesticks.getTimeFrame() == "5m") return 5;
		else if(candlesticks.getTimeFrame() == "1h") return 10;
		else if(candlesticks.getTimeFrame() == "4h") return 10;
		else if(candlesticks.getTimeFrame() == "1d") return 10;
		else return -1;	
	}

	void buyLong(Candlesticks& candlesticks) {
		//actually long

		double sizeUSD = calculateSizeUSD();
		int leverage = calcualteLeverage();

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = candlesticks.currentPrice * (1 - stopProfit);
		double takeProfit = candlesticks.currentPrice * (1 + stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "long", candlesticks.currentPrice, takeProfit, stopLoss, sizeUSD, leverage);
	}

	void sellShort(Candlesticks& candlesticks) {
		//actually short

		double sizeUSD = calculateSizeUSD();
		int leverage = calcualteLeverage();

		double stopProfit = calculateStopProfit(candlesticks);
		double stopLoss = candlesticks.currentPrice * (1 + stopProfit);
		double takeProfit = candlesticks.currentPrice * (1 - stopProfit);

		positions.addPosition(candlesticks.getTokenName(), candlesticks.getTimeFrame(), "short", candlesticks.currentPrice, takeProfit, stopLoss, sizeUSD, leverage);
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