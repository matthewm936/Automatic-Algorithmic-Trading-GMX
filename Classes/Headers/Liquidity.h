#ifndef LIQUIDITY_H
#define LIQUIDITY_H

#include <string>
#include "../nlohmann/json.hpp"

namespace Liquidity {
	// Forward declaration of json type from nlohmann library
	using json = nlohmann::json;

	// Function to run API command and return the response
	std::string runApiCommand(const std::string& command);

	// Function to parse JSON response from API command
	json getJsonResponse(const std::string& command);

	// Function to calculate bid-ask liquidity for a trading pair
	double getBidAskQty(std::string pairName);

	// Function to get 24-hour volume for a trading pair
	double get24hrVolume(std::string pairName);
}

#endif // LIQUIDITY_H
