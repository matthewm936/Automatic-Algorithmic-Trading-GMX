#include <deque>
#include <cmath>
#include <string>
#include "../nlohmann/json.hpp"

#include "Headers/Positions.h"
#include "Headers/Log.h"
#include "Headers/TradingPair.h"
#include "Headers/TradingPairs.h"
#include "Headers/TradingStrategy.h"
#include "Headers/RunCommand.h"

using namespace std;

double portfolioProfitLoss = 0;

Positions positions;
TradingPairs tradingPairs;

double TradingStrategy::calculateVariance(double* data, int size) {
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

double TradingStrategy::calculateVolatility(const deque<double>& prices, int start, int end) {
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

int TradingStrategy::consistentMovement(const TradingPair& pair, int minutesDuration, double targetStrength, deque<double> pricesInterval) {
	deque<double> prices = pricesInterval;

	int trendingStrength = 0;
	int totalIterations = 0;
	for(int i = 0; i < minutesDuration; i++) {
		totalIterations += 1;
		if(prices[i] > prices[i + 1]) {
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


double TradingStrategy::getAssetBalance(string asset) { 
	string result = runCommand((string("node mexc-api/asset-free-balance.js ") + asset).c_str());
	double resultDouble = stod(result);
	return resultDouble;
}

bool TradingStrategy::buy(string pairName, int amount) {
	TradingPair pair = tradingPairs.getPair(pairName);
	string result = runCommand((string("node mexc-api/buy.js ") + pairName + " " + to_string(amount)).c_str());
	if(result == "error") {
		return false;
	}

	string message = "pair " + pairName + " virtual buying rn bought " + to_string(amount) + " at price " + to_string(pair.getCurrentPrice());
	Log::log(message);
	return true;
}

bool TradingStrategy::sellAsset(const TradingPair& pair) {
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

bool TradingStrategy::stopLossTakeProfit(string asset, double stopPrice, double takeProfit) {
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

void TradingStrategy::trade(const TradingPair& pair) {
	string pairName = pair.pairName;


	if(!(pair.quoteAsset == "USDT"  || pair.quoteAsset == "USDC")) {
		return;
	} else if(pair.quoteVolume < 100000) {
		return;
	}

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

	if(pair.quoteVolume < 500000) {
		return;
	}

	if(pair.prices5minInterval.size() > 24) {
		int durationOfTrend5Min = 24; // x * 5 minutes 
		double strengthOfTrend = 0.75; // .x% of prices every 5 minutes increasing
		int trendingStrength = consistentMovement(pair, durationOfTrend5Min, strengthOfTrend, pair.prices5minInterval);

		if(trendingStrength > 0) {
			if(buy(pairName, 20)) {
				double percentChange40Mins = (pair.prices5minInterval[0] - pair.prices5minInterval[40]) / pair.prices5minInterval[40];
				double takeProfitMult = trendingStrength - 0.25;
				int takeProfitPercent = pow(percentChange40Mins * takeProfitMult, 2);
				double stopLoss = (1 - (percentChange40Mins / 2.0)) * pair.getCurrentPrice(); 
				double takeProfit = (1 + takeProfitPercent) * pair.getCurrentPrice();
				
				positions.addPosition(pairName, pair.prices1minInterval[0], takeProfit, stopLoss, -1);
				Log::logAndEmail("Bought " + pairName + " at " + to_string(pair.getCurrentPrice()) + " with take profit at " + to_string(takeProfit) + " and stop loss at " + to_string(stopLoss) + " with a trending strength of " + to_string(trendingStrength) + " over " + to_string(durationOfTrend5Min * 5) + " minutes");
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
