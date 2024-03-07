#ifndef PORTFOLIO_CPP
#define PORTFOLIO_CPP

#include "IndicatorDirection.cpp"
#include "Trade.cpp"

#include <cmath>
 
class Portfolio {
private:
	double returns;
	double positiveReturnTrade;

	int durationLong = 0;
	int durationShort = 0;

	IndicatorDirection currentDirection = IndicatorDirection::NONE;
	Trade trades = Trade();

	void totalReturns(double tradeReturn) {
		if(tradeReturn > 0) {
			this->positiveReturnTrade++;
		}
		this->returns *= (1 + tradeReturn);
	}

	void updateDuration(IndicatorDirection incomingDirection) {
		if (incomingDirection == IndicatorDirection::LONG) {
			durationLong++;
		} else if (incomingDirection == IndicatorDirection::SHORT) {
			durationShort++;
		}
	}

public:
	Portfolio() {
		this->positiveReturnTrade = 0.0;
		this->returns = 1;
	}

	void updatePortfolio(IndicatorDirection incomingDirection, double currentPrice) {
		updateDuration(incomingDirection);

		if(currentDirection == incomingDirection || incomingDirection == IndicatorDirection::NEUTRAL) {
			return;
		}
		if (currentDirection == IndicatorDirection::LONG && incomingDirection == IndicatorDirection::SHORT) {
			totalReturns(trades.exitLong(currentPrice));
			trades.enterShort(currentPrice);
		} else if (currentDirection == IndicatorDirection::SHORT && incomingDirection == IndicatorDirection::LONG) {
			totalReturns(trades.exitShort(currentPrice));
			trades.enterLong(currentPrice);
		} else if (currentDirection == IndicatorDirection::NONE) {
			if (incomingDirection == IndicatorDirection::LONG) {
				trades.enterLong(currentPrice);
			} else if (incomingDirection == IndicatorDirection::SHORT) {
				trades.enterShort(currentPrice);
			}
		}
		this->currentDirection = incomingDirection;
	}

	void closePortfolio(double currentPrice) {
		if (currentDirection == IndicatorDirection::LONG) {
			totalReturns(trades.exitLong(currentPrice));
		} else if (currentDirection == IndicatorDirection::SHORT) {
			totalReturns(trades.exitShort(currentPrice));
		} 
	}

	double getReturns() {
		return returns;
	}

	string getTradeStats() {
		string stats = "Portfolio returns %";
    	stats += to_string(returns * 100);
		stats += " win %";
    	stats += to_string(this->positiveReturnTrade / trades.getTrades() * 100.0);
		stats += " Duration Long/short: ";
		stats += to_string(durationLong) + "/";
		stats += to_string(durationShort);
		stats += " trades: ";
		stats += to_string(trades.getTrades());
		// stats += " Num longs/shorts: ";
		// stats += to_string(trades.getLongs()) + "/";
		// stats += to_string(trades.getShorts());
		return stats;
	}
};

#endif 
