	#ifndef TRADINGSTRATEGY_H
	#define TRADINGSTRATEGY_H

	#include <deque>
	#include <cmath>
	#include <string>
	#include "../nlohmann/json.hpp"
	#include "Positions.h"
	#include "Log.h"
	#include "TradingPair.h"
	#include "TradingPairs.h"

	class TradingStrategy {
	private:
		double portfolioProfitLoss = 0;

		double calculateVariance(double* data, int size);

		double calculateVolatility(const std::deque<double>& prices, int start, int end);

		double consistentMovement(const TradingPair& pair, int minutesDuration, std::deque<double> pricesInterval);

		friend class TradingStrategyTest;

	public:
		Positions positions;
		TradingPairs tradingPairs;

		TradingStrategy(Positions& pos, TradingPairs& pairs) : positions(pos), tradingPairs(pairs) {}

		double getAssetBalance(std::string asset);

		bool buy(std::string pairName, int amount);

		bool sellAsset(const TradingPair& pair);

		bool stopLossTakeProfit(std::string asset, double stopPrice, double takeProfit);

		void trade(const TradingPair& pair);
	};

	#endif // TRADINGSTRATEGY_H
