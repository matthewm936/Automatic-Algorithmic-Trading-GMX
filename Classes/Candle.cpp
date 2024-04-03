class Candle {
	public:
		time_t timeStamp;
		double open;
		double high;
		double low;
		double close;

		Candle(time_t timeStamp, double open, double high, double low, double close) {
			this->timeStamp = timeStamp;
			this->open = open;
			this->high = high;
			this->low = low;
			this->close = close;
		}

		
};
