#ifndef TradingStrategy_CPP
#define TradingStrategy_CPP

#include <deque>
// #include "nlohmann/json.hpp"

#include "Positions.cpp"
#include "Log.cpp"
#include "TradingPair.cpp"

class TradingStrategy { 
	
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
		int size = end - start + 1;
		vector<double> data(size);
		for (int i = 0; i < size; ++i) {
			data[i] = prices[start + i];
		}
		vector<double> returns(size - 1);
		for (int i = 0; i < size - 1; ++i) {
			returns[i] = log(data[i + 1] / data[i]);
		}
		return sqrt(calculateVariance(returns.data(), size - 1));
	}

	bool buySignal8XVolatiltyIn1Min(const TradingPair& pair) {
		deque<double> prices = pair.prices1minInterval;
		string pairName = pair.pairName;

		if(prices.size() < 6) {
			return false;
		}
		double volatility = calculateVolatility(prices, 1, 5);

		double THREASHHOLD = volatility * 8;

		double oneMinReturn = (prices[0] - prices[1])/ prices[1];

		if(oneMinReturn > THREASHHOLD && oneMinReturn > 0.05) {
			Log::log("Pair " + pairName + " buy signal w/ 1min8xvol " + to_string(oneMinReturn) + "\% volatility: " + to_string(volatility) + " quoteVol: " + to_string(pair.quoteVolume));

			double takeProfit = (1 + oneMinReturn * 0.6) * prices[0];
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
			double stopLoss = (1- fiveMinReturn * 1.5) * prices[0];
			time_t exitTime = time(0) + 60 * 10; // 10 minutes after the current time
			positions.addPosition(pairName, prices[0], takeProfit, stopLoss, exitTime);
			return true;
		} return false;
	}

public:	
	Positions positions;
	TradingStrategy(Positions positions) {
		this->positions = positions;
	}

	double getAssetBalance(string asset) {
		double result = stod(runCommand((string("node mexc-api/asset-free-balance.js ") + asset).c_str()));
		if(result == -1) {
			Log::log("Asset balance " + asset + " returned -1 through getAssetBalance with node mexc-api/asset-free-balance.js");
		}		
		return result;
	}

	bool buy(string pairName, int amount) {
		string result = runCommand((string("node mexc-api/buy.js ") + pairName + " " + to_string(amount)).c_str());
		Log::tradeLog("Pair " + pairName + " bought " + to_string(amount));
		return true;
	}
	
	bool sellAll(string pairName) {
		double assetBalance = getAssetBalance(pairName);
		if(assetBalance == -1) {
			Log::log("Asset balance " + pairName + " returned -1 through sellAll");
			return false;
		}
		string result = runCommand((string("node mexc-api/sell.js ") + pairName + " " + to_string(assetBalance)).c_str());
		Log::tradeLog("Pair " + pairName + " sold " + to_string(assetBalance));
		return true;
	}

	void trade(const TradingPair& pair) {
		string pairName = pair.pairName;

		if(positions.exists(pairName)) {
			Log::log("Pair " + pairName + " already in a position");

			// run sell here
			if(positions[pairName].stopLoss < pair.getCurrentPrice()) {
				positions.removePosition(pairName);
			}
			else if(time(0) > positions[pairName].exitTimeCondition) {
				positions.removePosition(pairName);
			}
			else if(positions[pairName].takeProfit < pair.getCurrentPrice()) {
				positions.removePosition(pairName);
				// system("node mexc-api/sell.js " + pairName + " 100");
			}
			return;
		}

		if (buySignal8XVolatiltyIn1Min(pair)) {
			// system("node mexc-api/buy.js " + pairName + " 20");
			// Log::tradeLog("Pair " + pairName + " bought at " + std::to_string(pair.getCurrentPrice()));
		}

		if (buySignal4XVolatiltyIn20Min(pair)) {
			// system("node mexc-api/buy.js " + pairName + " 30");
			// Log::tradeLog("Pair " + pairName + " bought at " + std::to_string(pair.getCurrentPrice()));
		}
	}

};

#endif
