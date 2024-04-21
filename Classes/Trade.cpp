#ifndef Trade_cpp
#define Trade_cpp

class Trade {
private:

public:
	Trade() {}

	void checkForTradeOpportunity(Candlesticks& candlesticks) {
		if(candlesticks.position) {
			return;
		} else {
			candlesticks.calculateCandleStatistics();
			buy(candlesticks.getTokenName());
	
		}
	}

	void buy(string pairname) {
		// runCommand("node temp");
	}

	void sell() {
	}
};

#endif