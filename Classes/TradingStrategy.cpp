#ifndef TradingStrategy_CPP
#define TradingStrategy_CPP

#include <deque>
#include <cmath>
#include "../nlohmann/json.hpp"

#include "Positions.cpp"
#include "Log.cpp"
#include "TradingPair.cpp"

class TradingStrategy { 
	
private:
	double portfolioProfitLoss = 0;

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

	int consistentMovement(const TradingPair& pair, int minutesDuration, double targetStrength) {
		deque<double> prices = pair.prices1minInterval;

		int trendingStrength = 0;
		int totalIterations = 0;
		for(int i = 0; i < minutesDuration; i+=3) {
			totalIterations += 1;
			if(prices[i] > prices[i + 3]) {
				trendingStrength++;
			}
		}
		double strengthPercent = trendingStrength / totalIterations;
		Log::log("consistent movement function on pair" + pair.pairName + " " + to_string(strengthPercent) + " " + to_string(targetStrength));
		if(strengthPercent > targetStrength) {
			Log::email("Pair " + pair.baseAsset + " has a trending strength of " + to_string(strengthPercent) + " over " + to_string(minutesDuration) + " minutes");
			return strengthPercent;
		}

		return 0;
	}

	friend class TradingStrategyTest;

public:	
	Positions positions;
	TradingStrategy(Positions positions) {
		this->positions = positions;
	}

	double getAssetBalance(string asset) { 
		string result = runCommand((string("node mexc-api/asset-free-balance.js ") + asset).c_str());
		double resultDouble = stod(result);
		return resultDouble;
	}

	bool buy(string pairName, int amount) {
		string result = runCommand((string("node mexc-api/buy.js ") + pairName + " " + to_string(amount)).c_str());
		if(result == "error") {
			string message = "Pair " + pairName + " returned error through buy with node mexc-api/buy.js, js call api to mexc buy caused the issue, either the asset doesn't exist, or hte api had some issue";
			Log::email(message.c_str());
			return false;
		}
		Log::tradeLog("Pair " + pairName + " bought " + to_string(amount));
		Log::email("Pair " + pairName + " bought " + to_string(amount));
		return true;
	}
	
	bool sellAsset(string asset) {
		double assetBalance = getAssetBalance(asset);
		if(assetBalance == -1) {
			Log::log("getAssetBalance failed " + asset + " returned -1");
			return false;
		}
		if(assetBalance == 0) {
			Log::log("Asset balance " + asset + " is 0, cant sell an asset with 0 balance");
			return false;
		}
		string result = runCommand((string("node mexc-api/sell.js ") + asset + " " + to_string(assetBalance)).c_str()); // this is wrong, you need pairName not asset

		if(result == "error") {
			Log::log("Sell error " + asset + " returned error through sellAsset with node mexc-api/sell.js, js call api to mexc sell caused the issue, either the asset doesn't exist, or hte api had some issue");
			return false;
		}
		Log::tradeLog("Pair " + asset + " sold " + to_string(assetBalance));
		return true;
	}

	bool stopLossTakeProfit(string asset, double stopPrice, double takeProfit) {
		double assetBalance = getAssetBalance(asset);
		if(assetBalance == -1) {
			Log::log("getAssetBalance failed " + asset + " returned -1");
			return false;
		}
		if(assetBalance == 0) {
			Log::log("Asset balance " + asset + " is 0, cant sell an asset with 0 balance");
			return false;
		}
		string result = runCommand((string("node mexc-api/stop-loss-take-profit.js ") + asset +  " " + to_string(stopPrice) + " " + to_string(takeProfit) + " " + to_string(assetBalance)).c_str());

		if(result == "error") {
			string message = "Stop loss, take profit error " + asset + " returned error through sellAsset with node stop-loss-take-profit.js, js call api to mexc sell caused the issue, either the asset doesn't exist, or hte api had some issue";
			Log::email(message.c_str());
			return false;
		}
		Log::tradeLog("Pair " + asset + " sold " + to_string(assetBalance));
		return true;
	}

	void trade(const TradingPair& pair) {
		string pairName = pair.pairName;

		if(positions.exists(pairName)) {
			Log::log("Pair " + pairName + " already in a position");
			return;
		}

		if(pair.prices1minInterval.size() > 45) {
			int durationOfTrendMin = 40; // x minutes
			double strengthOfTrend = 0.75; // .x% of prices every 3 minutes increasing
			int trendingStrength = consistentMovement(pair, durationOfTrendMin, strengthOfTrend);

			if(trendingStrength > 0) {
				// double percentChange40Mins = (pair.prices1minInterval[0] - pair.prices1minInterval[40]) / pair.prices1minInterval[40];
				// double takeProfitMult = trendingStrength - 0.25;
				// int takeProfitPercent = pow(percentChange40Mins * takeProfitMult, 2);
				if(buy(pairName, 20)) {
					positions.addPosition(pairName, pair.prices1minInterval[0]);

					// double stopLoss = (1 - (percentChange40Mins / 2.0)) * pair.getCurrentPrice(); 
					// double takeProfit = (1 + takeProfitPercent) * pair.getCurrentPrice();
					// stopLossTakeProfit(pairName, stopLoss, takeProfit); need to figure out how to do this api call, something weird about it
				} else {
					string message = "Failed to buy after hitting consistent movement of " + to_string(trendingStrength) + " on pair " + pair.baseAsset;
					Log::email(message.c_str());
				}
				return;
			} else {
				return;
			}
		}


	}

};

#endif
