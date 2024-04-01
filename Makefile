CXX = g++

CXXFLAGS = -Wall -std=c++17

SRCS = main.cpp Classes/Time.cpp Classes/Log.cpp Classes/Liquidity.cpp Classes/TradingStrategy.cpp Classes/TradingPair.cpp Classes/TradingPairs.cpp Classes/RunCommand.cpp Classes/Positions.cpp

EXEC = run

$(EXEC):
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRCS)

TEST_SRCS = tests/test.cpp

TEST_EXEC = test

$(TEST_EXEC):
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(TEST_SRCS)

clean:
	rm -f $(EXEC) $(TEST_EXEC) 