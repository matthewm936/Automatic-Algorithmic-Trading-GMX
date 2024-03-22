struct Position {
	std::string pairName;
	double entryPrice;
	double currentPrice;
	double profitLoss;

	time_t entryTime;
};

class Positions {
private:
	std::map<std::string, Position> positions;

	Positions() {}

public:
	static Positions& getInstance() {
		static Positions instance;
		return instance;
	}

	Positions(const Positions&) = delete;
	Positions& operator=(const Positions&) = delete;

	Position& operator[](const std::string& key) {
		return positions[key];
	}

	std::map<std::string, Position>::iterator find(const std::string& key) {
		return positions.find(key);
	}

	std::map<std::string, Position>::iterator end() {
		return positions.end();
	}
};