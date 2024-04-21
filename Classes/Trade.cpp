#ifndef Trade_cpp
#define Trade_cpp

class Trade {
private:

public:
	Trade() {}

	void checkForTradeOpportunity(Candlesticks& candlesticks) {
		if(candlesticks.cooldownDuration > 0) {
			candlesticks.cooldownDuration--;
			return;
		}
		if(!candlesticks.position) {
			candlesticks.calculateCandleStatistics(0, 4);

			int bullish = 0;
			if(candlesticks.greenCandlePercent >= .75) {
				bullish++;
			} 
			if(candlesticks.higherHighsPercent >= .75) {
				bullish++;
			} 
			if(candlesticks.higherLowsPercent >= .75) {
				bullish++;
			} 
			if(candlesticks.higherOpensPercent >= .75) {
				bullish++;
			} 
			if(candlesticks.higherClosesPercent >= .75) {
				bullish++;
			} 
			if(bullish >= 4) {
				buy(candlesticks.getTokenName());
				candlesticks.position = true; //leave position on sell? or can multiple signals arise and positons double donw?
				candlesticks.cooldownDuration = 5;

				Log::log("Bought " + candlesticks.getTokenName());
			}	
		}
	}

	void buy(string pairname) {
		// runCommand("node temp");
	}

	void sell() {
	}
};

#endif