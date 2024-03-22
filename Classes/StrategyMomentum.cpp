#ifndef StrategyMomentum_CPP
#define StrategyMomentum_CPP

#include <deque>

#include "Positions.cpp"
#include "Log.cpp"

class StrategyMomentum { 
	
private:

	void addPosition(string pairName, double entryPrice) {
		Positions& g_positions = Positions::getInstance();

		if(g_positions.find(pairName) != g_positions.end()) {
			Log::log("Pair" + pairName + " generated another buy signal but is already in a position");
			return;
		}
		Position position;
		position.pairName = pairName;
		position.entryPrice = entryPrice;
		position.entryTime = time(nullptr);
		g_positions[pairName] = position;

	}

	void buySignal20in45(deque<double>& prices, string pairName) {
		if(prices.size() < 45) {
			return;
		}
		if(prices[0] > prices[15] && prices[15] > prices[30] && prices[30] > prices[45] && (prices[0] > prices[45] * 1.20)) {
			addPosition(pairName, prices[0]);
		}
	}
	
	void buySignal10in15(deque<double>& prices, string pairName) {
		if(prices.size() < 15) {
			return;
		}
		if(prices[0] > prices[5] && prices[5] > prices[10] && prices[10] > prices[15] && (prices[0] > prices[15] * 1.10)) {
			addPosition(pairName, prices[0]);
		}
	}

public:	
	StrategyMomentum() {
	}

	void checkSignals(deque<double>& prices, string pairName) {
		buySignal20in45(prices, pairName);
		buySignal10in15(prices, pairName);
	}
};

#endif
