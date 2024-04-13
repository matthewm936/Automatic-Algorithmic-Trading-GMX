#ifndef Trade_cpp
#define Trade_cpp

#include <unordered_map>
#include <string>
#include <ctime>

#include "RunCommand.cpp"

class Trade {
private:

public:
	Trade() {}

	void buy(string pairname, double price, double size, int leverage, double takeProfit, double stopLoss, time_t exitTimeCondition) {
		runCommand();
	}

	void sell() {
	}
};

#endif