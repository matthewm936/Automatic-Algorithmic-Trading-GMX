#include <iostream>
#include <cassert>

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
    // candles added from api in descending order
    Candle candle11(1234567892, 100.0, 110.0, 90.0, 105.0);
    Candle candle21(1234567891, 105.0, 115.0, 95.0, 110.0);
    Candle candle31(1234567890, 110.0, 120.0, 100.0, 115.0);

    // aded in descending order but with repeats from above
    // like how the api data is fed into addCandle
    Candle candle1(1234567894, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567893, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1234567892, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1234567891, 115.0, 125.0, 105.0, 120.0);
    Candle candle5(1234567890, 120.0, 130.0, 110.0, 125.0);

    Candlesticks candlesticks("1h");
    candlesticks.addCandle(candle11);
    candlesticks.addCandle(candle21);
    candlesticks.addCandle(candle31);

    candlesticks.addCandle(candle1);
    candlesticks.addCandle(candle2);
    candlesticks.addCandle(candle3);
    candlesticks.addCandle(candle4);
    candlesticks.addCandle(candle5);

    candlesticks.checkCandleOrderCorrectness();

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
            assert(false);
            break;
        }
    }
}

void testGetTimeFrame() {
    std::string timeFrame = "1h";
    Candlesticks candlesticks(timeFrame);

    // Verify that the getTimeFrame() method returns the correct time frame
    assert(candlesticks.getTimeFrame() == timeFrame);
}

void testGetCandles() {
    Candle candle1(1234567890, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567891, 105.0, 115.0, 95.0, 110.0);

    Candlesticks candlesticks("1h");
    candlesticks.addCandle(candle1);
    candlesticks.addCandle(candle2);

    // Verify that the getCandles() method returns the correct deque of candles
    std::deque<Candle> candles = candlesticks.getCandles();
    assert(candles.size() == 2);
    assert(candles.front().timeStamp == candle2.timeStamp);
}

void testGetHighestCandle() {
    Candle candle1(1234567890, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567891, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1234567892, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1234567893, 115.0, 125.0, 105.0, 120.0);
    Candle candle5(1234567894, 150.0, 150.0, 110.0, 150.0);

    Candlesticks candlesticks("1h");
    candlesticks.addCandle(candle1);

    Candle highestCandle = candlesticks.getHighestCandle();
    assert(highestCandle.high == candle1.high);

    candlesticks.addCandle(candle2);

    highestCandle = candlesticks.getHighestCandle();
    assert(highestCandle.high == candle2.high);

    candlesticks.addCandle(candle3);

    highestCandle = candlesticks.getHighestCandle();
    assert(highestCandle.high == candle3.high);

    candlesticks.addCandle(candle4);

    highestCandle = candlesticks.getHighestCandle();
    assert(highestCandle.high == candle4.high);

    candlesticks.addCandle(candle5);

    highestCandle = candlesticks.getHighestCandle();
    assert(highestCandle.high == candle5.high);

    candlesticks.addCandle(candle4);

    highestCandle = candlesticks.getHighestCandle();
    assert(highestCandle.high == candle5.high);

    candlesticks.addCandle(candle4);
    candlesticks.addCandle(candle3);
    candlesticks.addCandle(candle1);

    highestCandle = candlesticks.getHighestCandle();
    assert(highestCandle.high == candle5.high);
}

void testGetLowestCandle() {
    Candle candle1(1234567890, 100.0, 115.0, 100.0, 105.0);
    Candle candle2(1234567891, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1234567892, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1234567893, 115.0, 125.0, 105.0, 120.0);
    Candle candle5(1234567894, 150.0, 150.0, 110.0, 150.0);

    Candlesticks candlesticks("1h");
    candlesticks.addCandle(candle1);
    candlesticks.addCandle(candle2);
    candlesticks.addCandle(candle3);
    candlesticks.addCandle(candle4);
    candlesticks.addCandle(candle5);

    // Verify that the getLowestCandle() method returns the candle with the lowest low value
    Candle lowestCandle = candlesticks.getLowestCandle();
    assert(lowestCandle.low == candle2.low);
}

void testGetGreenCandles() {
    Candle candle1(1234567890, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567891, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1234567892, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1234567893, 115.0, 125.0, 105.0, 120.0);
    Candle candle5(1234567894, 120.0, 130.0, 110.0, 125.0);

    Candlesticks candlesticks("1h");

    int greenCandles = candlesticks.getGreenCandles();
    assert(greenCandles == 0);

    candlesticks.addCandle(candle1);
    candlesticks.addCandle(candle2);
    candlesticks.addCandle(candle3);

    greenCandles = candlesticks.getGreenCandles();
    assert(greenCandles == 3);

    candlesticks.addCandle(candle4);

    greenCandles = candlesticks.getGreenCandles();
    assert(greenCandles == 4);

    candlesticks.addCandle(candle5);

    greenCandles = candlesticks.getGreenCandles();
    assert(greenCandles == 5);
}

void testGetRedCandles() {
    Candle candle1(1234567890, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567891, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1234567892, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1234567893, 115.0, 125.0, 105.0, 110.0);
    Candle candle5(1234567894, 120.0, 130.0, 110.0, 125.0);

    Candlesticks candlesticks("1h");

    int redCandles = candlesticks.getRedCandles();
    assert(redCandles == 0);  

    candlesticks.addCandle(candle1);

    redCandles = candlesticks.getRedCandles();
    assert(redCandles == 0);

    candlesticks.addCandle(candle2);

    redCandles = candlesticks.getRedCandles();
    assert(redCandles == 0);
    
    candlesticks.addCandle(candle3);
    candlesticks.addCandle(candle4);

    redCandles = candlesticks.getRedCandles();
    assert(redCandles == 1);
    
    candlesticks.addCandle(candle5);

    // Verify that the getRedCandles() method returns the correct number of red candles
    redCandles = candlesticks.getRedCandles();
    assert(redCandles == 1);
}

void testGetDojiCandles() {
    Candle candle1(1234567890, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567891, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1234567892, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1234567893, 115.0, 125.0, 105.0, 120.0);
    Candle candle5(1234567894, 125.0, 130.0, 110.0, 125.0);

    Candlesticks candlesticks("1h");

    int dojiCandles = candlesticks.getDojiCandles();
    assert(dojiCandles == 0);

    candlesticks.addCandle(candle1);

    dojiCandles = candlesticks.getDojiCandles();
    assert(dojiCandles == 0);

    candlesticks.addCandle(candle2);
    candlesticks.addCandle(candle3);
    candlesticks.addCandle(candle4);
    candlesticks.addCandle(candle5);

    dojiCandles = candlesticks.getDojiCandles();
    assert(dojiCandles == 1);
}

void testCheckCandleOrderCorrectness() {
    // candles added from api in descending order
    Candle candle11(1234567892, 100.0, 110.0, 90.0, 105.0);
    Candle candle21(1234567891, 105.0, 115.0, 95.0, 110.0);
    Candle candle31(1234567890, 110.0, 120.0, 100.0, 115.0);

    // aded in descending order but with repeats from above
    // like how the api data is fed into addCandle
    Candle candle1(1234567894, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1234567893, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1234567892, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1234567891, 115.0, 125.0, 105.0, 120.0);
    Candle candle5(1234567890, 120.0, 130.0, 110.0, 125.0);

    Candlesticks candlesticks("1h");
    candlesticks.addCandle(candle11);
    candlesticks.addCandle(candle21);
    candlesticks.addCandle(candle31);

    candlesticks.addCandle(candle1);
    candlesticks.addCandle(candle2);
    candlesticks.addCandle(candle3);
    candlesticks.addCandle(candle4);
    candlesticks.addCandle(candle5);

    // Verify that the checkCandleOrderCorrectness() method throws an exception if candles are not in the correct order
    candlesticks.checkCandleOrderCorrectness();

    for(size_t i = 1; i < candlesticks.getCandles().size() - 1; ++i) {
        if(candlesticks.getCandles()[i-1].timeStamp > candlesticks.getCandles()[i].timeStamp && candlesticks.getCandles()[i].timeStamp > candlesticks.getCandles()[i+1].timeStamp) {
        } else {
            std::cout << "Error: Candles are not in the correct order" << std::endl;
            assert(false);
            break;
        }
    }
}

void testCheckCandleMissingness() {
    Candle candle11(1712177760, 100.0, 110.0, 90.0, 105.0);
    Candle candle21(1712177700, 105.0, 115.0, 95.0, 110.0);

    Candle candle1(1712177880, 100.0, 110.0, 90.0, 105.0);
    Candle candle2(1712177820, 105.0, 115.0, 95.0, 110.0);
    Candle candle3(1712177760, 110.0, 120.0, 100.0, 115.0);
    Candle candle4(1712177700, 115.0, 125.0, 105.0, 120.0);

    Candlesticks candlesticks("1m");
    candlesticks.addCandle(candle11);
    candlesticks.addCandle(candle21);

    candlesticks.addCandle(candle1);
    candlesticks.addCandle(candle2);
    candlesticks.addCandle(candle3);
    candlesticks.addCandle(candle4);

    candlesticks.checkCandleOrderCorrectness();
    candlesticks.checkCandleMissingness();
}

int main() {
    testCandlesticksConstructor();
    testCandlesticksConstructorWithTimeFrame();
    testAddCandle();
    testCandleOrder();
    testGetTimeFrame();
    testGetCandles();
    testGetHighestCandle();
    testGetLowestCandle();

    testGetGreenCandles();
    testGetRedCandles();
    testGetDojiCandles();
    testCheckCandleOrderCorrectness();

    return 0;
}