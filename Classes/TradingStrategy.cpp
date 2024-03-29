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
		if(strengthPercent > targetStrength) {
			Log::log("Pair " + pair.baseAsset + " has a trending strength of " + to_string(strengthPercent) + " over " + to_string(minutesDuration) + " minutes");
			return strengthPercent;
		}

		return 0;
	}

	friend class TradingStrategyTest;

public:	
	Positions positions;
	TradingPairs tradingPairs;

	TradingStrategy(Positions& pos, TradingPairs& pairs) : positions(pos), tradingPairs(pairs) {

	double getAssetBalance(string asset) { 
		string result = runCommand((string("node mexc-api/asset-free-balance.js ") + asset).c_str());
		double resultDouble = stod(result);
		return resultDouble;
	}

	bool buy(string pairName, int amount) {
		TradingPair pair = tradingPairs.getPair(pairName);
		if(!(pair.quoteAsset == "USDT"  || pair.quoteAsset == "USDC")) {
			Log::log("NOT USDC or USDT " + pairName + " generated a buy but is not a USDT or USDC pair still attempting to buy");
		} else if(pair.quoteVolume < 10000) {
			Log::log("LOW quote volume " + pairName + " generated a buy but has a quote volume of less than 50000");
			return false;
		}
		string result = runCommand((string("node mexc-api/buy.js ") + pairName + " " + to_string(amount)).c_str());
		if(result == "error") {
			return false;
		}

		string message = "pair " + pairName + " virtual buying rn bought " + to_string(amount) + " at price " + to_string(pair.getCurrentPrice());
		Log::log(message);
		return true;
	}
	
	bool sellAsset(const TradingPair& pair) {
		double assetBalance = getAssetBalance(pair.baseAsset);
		string pairName = pair.pairName;

		if(assetBalance == -1) {
			Log::log("getAssetBalance failed " + pairName + " returned -1");
			return false;
		}
		if(assetBalance == 0) {
			Log::log("Asset balance " + pairName + " is 0, cant sell an asset with 0 balance");
			return false;
		}
		string result = runCommand((string("node mexc-api/sell.js ") + pairName + " " + to_string(assetBalance)).c_str());

		if(result == "error") {
			return false;
		}
		Log::log("Pair " + pairName + " sold " + to_string(assetBalance));
		positions.removePosition(pairName);
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
			return false;
		}
		Log::LogWithTimestamp("Pair " + asset + " sold " + to_string(assetBalance));
		return true;
	}

	void trade(const TradingPair& pair) {
		string pairName = pair.pairName;

		if(positions.exists(pairName)) {
			Log::log("Pair " + pairName + " already in a position");

			if(positions[pairName].takeProfit != -1 && positions[pairName].stopLoss != -1) {
				if(pair.getCurrentPrice() >= positions[pairName].takeProfit) {
					Log::LogWithTimestamp("Pair " + pairName + " hit take profit");
					sellAsset(pair);
				} else if(pair.getCurrentPrice() <= positions[pairName].stopLoss) {
					Log::LogWithTimestamp("Pair " + pairName + " hit stop loss");
					sellAsset(pair);
				}
			}
			return;
		}

		if(pair.prices1minInterval.size() > 45) {
			int durationOfTrendMin = 34; // x minutes
			double strengthOfTrend = 0.85; // .x% of prices every 3 minutes increasing
			int trendingStrength = consistentMovement(pair, durationOfTrendMin, strengthOfTrend);

			if(trendingStrength > 0) {
				if(buy(pairName, 20)) {
					double percentChange40Mins = (pair.prices1minInterval[0] - pair.prices1minInterval[40]) / pair.prices1minInterval[40];
					double takeProfitMult = trendingStrength - 0.25;
					int takeProfitPercent = pow(percentChange40Mins * takeProfitMult, 2);
					double stopLoss = (1 - (percentChange40Mins / 2.0)) * pair.getCurrentPrice(); 
					double takeProfit = (1 + takeProfitPercent) * pair.getCurrentPrice();
					
					positions.addPosition(pairName, pair.prices1minInterval[0], takeProfit, stopLoss);
				}
				else {
					string message = "Failed to buy after hitting consistent movement of " + to_string(trendingStrength) + " on pair " + pair.baseAsset;
					Log::LogWithTimestamp(message.c_str());
				}
				return;
			} else {
				return;
			}
		}


	}

};

#endif
