#include <iostream>
#include "../Classes/Candle.cpp"

void testCandleConstructor() {
    time_t timeStamp = time(nullptr);
    double open = 100.0;
    double high = 110.0;
    double low = 90.0;
    double close = 105.0;

    Candle candle(timeStamp, open, high, low, close);

    // Verify that the values are set correctly
    if (candle.timeStamp != timeStamp) {
        std::cout << "Error: Incorrect time stamp" << std::endl;
    }
    if (candle.open != open) {
        std::cout << "Error: Incorrect open value" << std::endl;
    }
    if (candle.high != high) {
        std::cout << "Error: Incorrect high value" << std::endl;
    }
    if (candle.low != low) {
        std::cout << "Error: Incorrect low value" << std::endl;
    }
    if (candle.close != close) {
        std::cout << "Error: Incorrect close value" << std::endl;
    }
}

void testCandleDefaultConstructor() {
    Candle candle;

    // Verify that the default values are set correctly
    if (candle.timeStamp != 0) {
        std::cout << "Error: Incorrect default time stamp" << std::endl;
    }
    if (candle.open != 0.0) {
        std::cout << "Error: Incorrect default open value" << std::endl;
    }
    if (candle.high != 0.0) {
        std::cout << "Error: Incorrect default high value" << std::endl;
    }
    if (candle.low != 0.0) {
        std::cout << "Error: Incorrect default low value" << std::endl;
    }
    if (candle.close != 0.0) {
        std::cout << "Error: Incorrect default close value" << std::endl;
    }
}

void testCandleWickRatio() {
    Candle candle(0, 100.0, 110.0, 90.0, 105.0);

    // Verify that the wick ratio is calculated correctly
    if (candle.getWickRatio() != 0.2) {
        std::cout << "Error: Incorrect wick ratio" << std::endl;
    }
}

int main() {
    testCandleConstructor();
    testCandleDefaultConstructor();

    testCandleWickRatio();

    return 0;
}