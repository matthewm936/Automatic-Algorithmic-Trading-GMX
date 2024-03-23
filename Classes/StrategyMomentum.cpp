#ifndef StrategyMomentum_CPP
#define StrategyMomentum_CPP

#include <deque>

#include "Positions.cpp"
#include "Log.cpp"
#include "TradingPair.cpp"

class StrategyMomentum { 
	
private:

	double portfolioProfitLoss = 0;

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
			data[i] = prices[start + i];
		}
		double returns[size - 1];
		for (int i = 0; i < size - 1; ++i) {
			returns[i] = log(data[i + 1] / data[i]);
		}
		return sqrt(calculateVariance(returns, size - 1));
	}

	bool buySignal6XVolatiltyIn1Min(const TradingPair& pair) {
		deque<double> prices = pair.prices1minInterval;
		string pairName = pair.pairName;

		if(prices.size() < 6) {
			return false;
		}
		double volatility = calculateVolatility(prices, 1, 5);

		double THREASHHOLD = volatility * 6;

		double oneMinReturn = (prices[0] - prices[1])/ prices[1];

		if(oneMinReturn > THREASHHOLD && oneMinReturn > 0.05) {
			Log::log("Pair " + pairName + " buy signal w/ 1min4xvol " + to_string(oneMinReturn) + "\% volatility: " + to_string(volatility) + " quoteVol: " + to_string(pair.quoteVolume));

			double takeProfit = oneMinReturn * 0.6 * prices[0];
			double stopLoss = oneMinReturn * 0.5 * prices[0];
			time_t exitTime = time(0) + 60 * 5; // 10 minutes after the current time
			positions.addPosition(pairName, prices[0], takeProfit, stopLoss, exitTime);
			return true;
		} return false;
	}

	bool buySignal4XVolatiltyIn20Min(const TradingPair& pair) {
		deque<double> prices = pair.prices1minInterval;
		string pairName = pair.pairName;

		if(prices.size() < 26) {
			return false;
		}
		double volatility = calculateVolatility(prices, 5, 25);

		double THREASHHOLD = volatility * 4;

		double fiveMinReturn = (prices[0] - prices[5])/ prices[5];

		if(fiveMinReturn > THREASHHOLD && fiveMinReturn > 0.05) {
			Log::log("Pair " + pairName + " buy signal w/ 5min3xvol " + to_string(fiveMinReturn) + "\% volatility: " + to_string(volatility) + " quoteVol: " + to_string(pair.quoteVolume));

			double takeProfit = fiveMinReturn * 2 * prices[0];
			double stopLoss = fiveMinReturn * 1.5 * prices[0];
			time_t exitTime = time(0) + 60 * 10; // 10 minutes after the current time
			positions.addPosition(pairName, prices[0], takeProfit, stopLoss, exitTime);
			return true;
		} return false;
	}

public:	
	Positions positions;
	StrategyMomentum(Positions positions) {
		this->positions = positions;
	}

	void trade(const TradingPair& pair) {
		if(positions.exists(pair.pairName)) {
			Log::log("Pair " + pair.pairName + " already in a position");

			// run sell here
			double percentProfitLoss = (pair.getCurrentPrice() - positions[pair.pairName].entryPrice) / positions[pair.pairName].entryPrice;
			if(positions[pair.pairName].stopLoss < pair.getCurrentPrice()) {
				Log::tradeLog("Pair " + pair.pairName + " STOP LOSS:" + std::to_string(percentProfitLoss) + ". Position: Entry Price - " + std::to_string(positions[pair.pairName].entryPrice) + ", Stop Loss - " + std::to_string(positions[pair.pairName].stopLoss));
				positions.removePosition(pair.pairName);

				portfolioProfitLoss += percentProfitLoss;
			}
			else if(time(0) > positions[pair.pairName].exitTimeCondition) {
				Log::tradeLog("Pair " + pair.pairName + " EXIT TIME CONDITION:" + std::to_string(percentProfitLoss) + ". Position: Entry Price - " + std::to_string(positions[pair.pairName].entryPrice) + ", Exit Time Condition - " + std::to_string(positions[pair.pairName].exitTimeCondition));
				positions.removePosition(pair.pairName);

				portfolioProfitLoss += percentProfitLoss;
			}
			else if(positions[pair.pairName].takeProfit < pair.getCurrentPrice()) {
				Log::tradeLog("Pair " + pair.pairName + " TAKE PROFIT:" + std::to_string(percentProfitLoss) + ". Position: Entry Price - " + std::to_string(positions[pair.pairName].entryPrice) + ", Take Profit - " + std::to_string(positions[pair.pairName].takeProfit));
				positions.removePosition(pair.pairName);
				// system("node mexc-api/sell.js " + pair.pairName + " 100");

				portfolioProfitLoss += percentProfitLoss;
			}
			return;

			Log::log("Portfolio pnl: " + std::to_string(portfolioProfitLoss));
		}

		if (buySignal6XVolatiltyIn1Min(pair)) {
			// system("node mexc-api/buy.js " + pair.pairName + " 20");
			// Log::tradeLog("Pair " + pair.pairName + " bought at " + std::to_string(pair.getCurrentPrice()));
		}

		if (buySignal4XVolatiltyIn20Min(pair)) {
			// system("node mexc-api/buy.js " + pair.pairName + " 30");
			// Log::tradeLog("Pair " + pair.pairName + " bought at " + std::to_string(pair.getCurrentPrice()));
		}
	}

};

#endif
