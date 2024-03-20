#ifndef StrategyMomentum_CPP
#define StrategyMomentum_CPP

#include <deque>

using namespace std;

class StrategyMomentum { 
	
private:

public:	
	StrategyMomentum() {
	}

	bool buySignal20in45(deque<double>& prices) {
		if(prices.size() < 45) {
			return false;
		}
		return prices[0] > prices[15] && prices[15] > prices[30] && prices[30] > prices[45] && (prices[0] > prices[45] * 1.20);
	}
	
	bool buySignal10in15(deque<double>& prices) {
		if(prices.size() < 15) {
			return false;
		}
		return prices[0] > prices[5] && prices[5] > prices[10] && prices[10] > prices[15] && (prices[0] > prices[15] * 1.10);
	}

};
#endif
