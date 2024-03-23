struct Position {
	std::string pairName;
	double entryPrice;
	double currentPrice;
	double profitLoss;

	time_t entryTime;
};

class Positions {
private:
	std::unordered_map<std::string, Position> positions;

public:
	Positions() {}

	Position& operator[](const std::string& key) {
		return positions[key];
	}

	std::unordered_map<std::string, Position>::iterator find(const std::string& key) {
		return positions.find(key);
	}

	std::unordered_map<std::string, Position>::iterator end() {
		return positions.end();
	}

	void addPosition(const std::string& pairName, double entryPrice) {
		Position position;
		position.pairName = pairName;
		position.entryPrice = entryPrice;
		position.entryTime = time(0);
		positions[pairName] = position;
	}

	void removePosition(const std::string& pairName) {
		positions.erase(pairName);
	}

	size_t size() const {
		return positions.size();
	}

	// bool buy() {
			// void addPosition(string pairName, double entryPrice) {
	// 	Positions& g_positions = Positions::getInstance();

	// 	if(g_positions.find(pairName) != g_positions.end()) {
	// 		Log::log("Pair" + pairName + " generated another buy signal but is already in a position");
	// 		return;
	// 	}
	// 	if (pairName.find("USDT") == string::npos) {
	// 		Log::log("Pair " + pairName + " generated buy signal but is not a USDT pair");
	// 		return;
	// 	}

	// 	double quoteVolume = g_Trading_Pairs.pairs[pairName].getQuoteVolume();
	// 	int positionSizeUSDT = 0;

	// 	if(quoteVolume < 50000) {
	// 		positionSizeUSDT = 20;
	// 	} if(quoteVolume < 10000) {
	// 		positionSizeUSDT = 10;
	// 	} 
	// 	else {
	// 		positionSizeUSDT = 40;
	// 	}

	// 	int exitStatus = system(("node mexc-api/buy.js " + pairName + " " + std::to_string(positionSizeUSDT)).c_str());

	// 	if (exitStatus != 0) {
	// 		Log::log("Error buying " + pairName);
	// 		throw std::runtime_error("Command failed with exit status " + std::to_string(exitStatus));
	// 	}

	// 	Log::tradeLog(pairName + "," + to_string(entryPrice) + "," + to_string(time(nullptr)) + ",BUY");
	// }
	// }
};