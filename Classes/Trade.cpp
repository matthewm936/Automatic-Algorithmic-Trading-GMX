#ifndef Trade_cpp
#define Trade_cpp

class Trade {
private:

public:
	Trade() {}

	void checkForTradeOpportunity(Candlesticks& candlesticks) {
		if(!candlesticks.position) {
			if(strategyConsistent()) {
				buy(candlesticks.getTokenName());
				candlesticks.position = true; // curerntly no logic ot leave position
				candlesticks.entryPrice = candlesticks.getCurrentPrice();
				
				Log::log("Bought " + candlesticks.getTokenName());	
			}
		}
	}

	bool strategyConsistent() {
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
	}

	void buy(string pairname) {
		// runCommand("node temp");
	}

	void sell() {
	}
};

#endif