#include <iostream>
#include "../Classes/Candlesticks.cpp"
using std::cout;
using std::endl;

void testCandlesticksConstructor() {
    Candlesticks candlesticks;

    // Verify that the default values are set correctly
    if (candlesticks.getTimeFrame() != "") {
        std::cout << "Error: Incorrect default time frame" << std::endl;
    }
    if (candlesticks.getCandles().size() != 0) {
        std::cout << "Error: Incorrect default number of candles" << std::endl;
    }
}

void testCandlesticksConstructorWithTimeFrame() {
    std::string timeFrame = "1h";
    Candlesticks candlesticks(timeFrame);

    // Verify that the time frame is set correctly
    if (candlesticks.getTimeFrame() != timeFrame) {
        std::cout << "Error: Incorrect time frame" << std::endl;
    }
    if (candlesticks.getCandles().size() != 0) {
        std::cout << "Error: Incorrect default number of candles" << std::endl;
    }
}

void testAddCandle() {
    Candle candle1(1234567890, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567891, 105.0, 115.0, 95.0, 110.0);

    Candlesticks candlesticks("1h");
    candlesticks.addCandle(candle1);
    candlesticks.addCandle(candle2);

    // Verify that the candles are added correctly
    std::deque<Candle> candles = candlesticks.getCandles();
    if (candles.size() != 2) {
        std::cout << "Error: Incorrect number of candles" << std::endl;
    }
    if (candles.front().timeStamp != candle2.timeStamp) {
        std::cout << "Error: Incorrect order of candles" << std::endl;
    }
}

void testCandleOrder() {
    Candle candle1(1234567890, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567891, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1234567892, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1234567893, 115.0, 125.0, 105.0, 120.0);
    Candle candle5(1234567894, 120.0, 130.0, 110.0, 125.0);

    cout << "Candle 1: " << candle1.timeStamp << endl;
    cout << "Candle 2: " << candle2.timeStamp << endl;
    cout << "Candle 3: " << candle3.timeStamp << endl;
    cout << "Candle 4: " << candle4.timeStamp << endl;
    cout << "Candle 5: " << candle5.timeStamp << endl;

    Candlesticks candlesticks("1h");
    candlesticks.addCandle(candle1);
    candlesticks.addCandle(candle2);
    candlesticks.addCandle(candle3);
    candlesticks.addCandle(candle4);
    candlesticks.addCandle(candle5);

    // Verify that the candles are in the correct order
    std::deque<Candle> candles = candlesticks.getCandles();

    cout << candles[0].timeStamp << endl;
    cout << candles[1].timeStamp << endl;

    if(candles.size() != 5) {
        std::cout << "Error: Incorrect number of candles" << std::endl;
    }

    if(candles[0].timeStamp < candles[1].timeStamp) {
        std::cout << "Error: Candles are not in the correct order" << std::endl;
    }

    for (size_t i = 1; i < candles.size(); ++i) {
        if (candles[i-1].timeStamp < candles[i].timeStamp) {
            std::cout << "Error: Candles are not in the correct order" << std::endl;
            break;
        }
    }
}

int main() {
    testCandlesticksConstructor();
    testCandlesticksConstructorWithTimeFrame();
    testAddCandle();
    testCandleOrder();

    return 0;
}