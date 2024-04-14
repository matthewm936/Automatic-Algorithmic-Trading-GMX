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

	// for now buy 1k worth of token at market price and sell
	// after several candles
	void buy(string pairname) {
		runCommand();
	}

	void sell() {
	}
};

#endif