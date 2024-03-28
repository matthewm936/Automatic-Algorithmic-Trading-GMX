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
			Log::email("Pair " + pair.baseAsset + " has a trending strength of " + to_string(strengthPercent) + " over " + to_string(minutesDuration) + " minutes");
			return strengthPercent;
		}

		return 0;
	}

	friend class TradingStrategyTest;

public:	
	Positions positions;
	TradingPairs tradingPairs;

	TradingStrategy(Positions pos, TradingPairs pairs) {
		this->positions = pos;
		this->tradingPairs = pairs;
	}

	double getAssetBalance(string asset) { 
		string result = runCommand((string("node mexc-api/asset-free-balance.js ") + asset).c_str());
		double resultDouble = stod(result);
		return resultDouble;
	}

	bool buy(string pairName, int amount) {
		TradingPair pair = tradingPairs.getPair(pairName);
		if(!(pair.quoteAsset == "USDT"  || pair.quoteAsset == "USDC")) {
			Log::log("Pair " + pairName + " generated a buy but is not a USDT or USDC pair still attempting to buy");
		} else if(pair.quoteVolume < 50000) {
			Log::log("Pair " + pairName + " generated a buy but has a quote volume of less than 10,000 still attempint to buy");
		}
		string result = runCommand((string("node mexc-api/buy.js ") + pairName + " " + to_string(amount)).c_str());
		if(result == "error") {
			string message = "Pair " + pairName + " returned error through buy with node mexc-api/buy.js, js call api to mexc buy caused the issue, either the asset doesn't exist, or hte api had some issue";
			Log::email(message.c_str());
			return false;
		}

		string message = "pair " + pairName + " virtual buying rn bought " + to_string(amount);
		Log::tradeLog(message);
		Log::email(message);
		return true;
	}
	
	bool sellAsset(const TradingPair& pair) {
		double assetBalance = getAssetBalance(pair.baseAsset);
		string pairName = pair.pairName;

		if(assetBalance == -1) {
			Log::log("getAssetBalance failed " + pairName + " returned -1");
			Log::email("getAssetBalance failed " + pairName + " returned -1");
			return false;
		}
		if(assetBalance == 0) {
			Log::log("Asset balance " + pairName + " is 0, cant sell an asset with 0 balance");
			Log::email("Asset balance " + pairName + " is 0, cant sell an asset with 0 balance");
			return false;
		}
		string result = runCommand((string("node mexc-api/sell.js ") + pairName + " " + to_string(assetBalance)).c_str()); // this is wrong, you need pairName not asset

		if(result == "error") {
			Log::log("Sell error " + pairName + " returned error through sellAsset with node mexc-api/sell.js, js call api to mexc sell caused the issue, either the asset doesn't exist, or hte api had some issue");
			Log::email("Sell error " + pairName + " returned error through sellAsset with node mexc-api/sell.js, js call api to mexc sell caused the issue, either the asset doesn't exist, or hte api had some issue");
			return false;
		}
		Log::tradeLog("Pair " + pairName + " sold " + to_string(assetBalance));
		Log::email("Pair " + pairName + " sold " + to_string(assetBalance));
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

			if(positions[pairName].takeProfit != -1 && positions[pairName].stopLoss != -1) {
				if(pair.getCurrentPrice() >= positions[pairName].takeProfit) {
					Log::log("Pair " + pairName + " hit take profit");
					sellAsset(pair);
				} else if(pair.getCurrentPrice() <= positions[pairName].stopLoss) {
					Log::log("Pair " + pairName + " hit stop loss");
					sellAsset(pair);
				}
			}
			return;
		}

		if(pair.prices1minInterval.size() > 45) {
			int durationOfTrendMin = 40; // x minutes
			double strengthOfTrend = 0.75; // .x% of prices every 3 minutes increasing
			int trendingStrength = consistentMovement(pair, durationOfTrendMin, strengthOfTrend);

			if(trendingStrength > 0) {
				if(buy(pairName, 20)) {
					double percentChange40Mins = (pair.prices1minInterval[0] - pair.prices1minInterval[40]) / pair.prices1minInterval[40];
					double takeProfitMult = trendingStrength - 0.25;
					int takeProfitPercent = pow(percentChange40Mins * takeProfitMult, 2);
					double stopLoss = (1 - (percentChange40Mins / 2.0)) * pair.getCurrentPrice(); 
					double takeProfit = (1 + takeProfitPercent) * pair.getCurrentPrice();
					
					positions.addPosition(pairName, pair.prices1minInterval[0], takeProfit, stopLoss);

					Log::email(" added position " + pairName + " with take profit " + to_string(takeProfit) + " and stop loss " + to_string(stopLoss));
				}
				else {
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
