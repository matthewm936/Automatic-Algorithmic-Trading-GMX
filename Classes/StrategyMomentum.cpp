#ifndef StrategyMomentum_CPP
#define StrategyMomentum_CPP

#include <deque>

#include "Positions.cpp"
#include "Log.cpp"
#include "TradingPair.cpp"

class StrategyMomentum { 
	
private:

	// double getVolatility(deque<double>& prices) {
	// 	double sum = 0;
	// 	double mean = 0;
	// 	for(int i = 0; i < prices.size(); i++) {
	// 		sum += prices[i];
	// 	}
	// 	mean = sum / prices.size();
	// 	double sumOfSquares = 0;
	// 	for(int i = 0; i < prices.size(); i++) {
	// 		sumOfSquares += (prices[i] - mean) * (prices[i] - mean);
	// 	}
	// 	return sqrt(sumOfSquares / prices.size());
	// }

	double calculateVariance(double* data, int size) {
		double sum = 0.0, mean, variance = 0.0;
		for(int i = 0; i < size; ++i) {
			sum += data[i];
		}
		mean = sum/size;

		for(int i = 0; i < size; ++i) {
			variance += pow(data[i] - mean, 2);
		}
		return variance / size;
	}

	double calculateVolatility(const deque<double>& prices, int start, int end) {
		const int size = end - start + 1;
		double data[size];
		for (int i = 0; i < size; ++i) {
			data[i] = prices[start];
		}
		double returns[size - 1];
		for (int i = 0; i < size - 1; ++i) {
			returns[i] = log(data[i + 1] / data[i]);
		}
		return sqrt(calculateVariance(returns, size - 1));
	}

	// one minute return 4x the volatility of the past 5 minutes
	bool buySignal4XVolatiltyIn1Min(const TradingPair& pair) {
		deque<double> prices = pair.prices1minInterval;
		string pairName = pair.pairName;

		if(prices.size() < 6) {
			return false;
		}
		double volatility = calculateVolatility(prices, 1, 5);

		double THREASHHOLD = volatility * 4;

		double oneMinReturn = (prices[0] - prices[1])/ prices[1];

		if(oneMinReturn > THREASHHOLD) {
			Log::log("Pair " + pairName + " generated a buy signal with a 1 minute return of " + to_string(oneMinReturn) + " and a volatility of " + to_string(volatility));
			return true;
		} return false;
	}

	// 5 minute return 3x volatility of the past 20 minutes
	bool buySignal3XVolatiltyIn20Min(const TradingPair& pair) {
		deque<double> prices = pair.prices1minInterval;
		string pairName = pair.pairName;

		if(prices.size() < 26) {
			return false;
		}
		double volatility = calculateVolatility(prices, 5, 25);

		double THREASHHOLD = volatility * 3;

		double fiveMinReturn = (prices[0] - prices[5])/ prices[5];

		if(fiveMinReturn > THREASHHOLD) {
			Log::log("Pair " + pairName + " generated a buy signal with a 5 minute return of " + to_string(fiveMinReturn) + " and a volatility of " + to_string(volatility));
			return true;
		} return false;
	}

public:	
	StrategyMomentum() {
	}

	std::vector<std::string> checkSignals(const TradingPair& pair) {
		std::vector<std::string> triggeredSignals;

		if (buySignal4XVolatiltyIn1Min(pair)) {
			triggeredSignals.push_back("buySignal4XVolatiltyIn1Min");
		}

		if (buySignal3XVolatiltyIn20Min(pair)) {
			triggeredSignals.push_back("buySignal3XVolatiltyIn20Min");
		}

		return triggeredSignals;
	}
};

#endif
