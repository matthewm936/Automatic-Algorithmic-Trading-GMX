#include <deque>
#include <cmath>
#include <string>
#include <iostream>
#include <string>

#include "Positions.cpp"
#include "RunCommand.cpp"

using std::string;
using std::deque;
using std::to_string;

class TradingStrategy {
public:
	TradingStrategy() {}

	double calculateVariance(double* data, int size);
	double calculateVolatility(const std::deque<double>& prices, int start, int end);
	// double consistentMovement();

private:
};

double TradingStrategy::calculateVariance(double* data, int size) {
	double sum = 0.0, mean, variance = 0.0;
	for (int i = 0; i < size; ++i) {
		sum += data[i];
	}
	mean = sum / size;

	for (int i = 0; i < size; ++i) {
		variance += pow(data[i] - mean, 2);
	}
	return variance / size;
}

double TradingStrategy::calculateVolatility(const std::deque<double>& prices, int start, int end) {
	int size = end - start + 1;
	std::vector<double> data(size);
	for (int i = 0; i < size; ++i) {
		data[i] = prices[start + i];
	}
	std::vector<double> returns(size - 1);
	for (int i = 0; i < size - 1; ++i) {
		returns[i] = log(data[i + 1] / data[i]);
	}
	return sqrt(calculateVariance(returns.data(), size - 1));
}

// double TradingStrategy::consistentMovement(const TradingPair& pair, int minutesDuration, std::deque<double> pricesInterval) {
// 	std::deque<double> prices = pricesInterval;

// 	int trendingStrength = 0;
// 	int totalIterations = 0;
// 	for (int i = 0; i < minutesDuration; i++) {
// 		totalIterations += 1;
// 		if (prices[i] > prices[i + 1]) {
// 			trendingStrength++;
// 		}
// 	}
// 	double strengthPercent = trendingStrength / totalIterations;
// 	return strengthPercent;
// }