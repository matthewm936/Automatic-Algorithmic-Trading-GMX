CXX = g++
CXXFLAGS = -Wall -std=c++17
SRCS = main.cpp Classes/Time.cpp Classes/Log.cpp Classes/TradingStrategy.cpp Classes/TradingPair.cpp Classes/TradingPairs.cpp Classes/RunCommand.cpp Classes/Positions.cpp
EXEC = run
TEST_SRCS = tests/test.cpp
TEST_EXEC = test

$(EXEC): $(SRCS)
	@echo "Compiling $@..."
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilation complete."

$(TEST_EXEC): $(TEST_SRCS)
	@echo "Compiling $@..."
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilation complete."

clean:
	rm -f $(EXEC) $(TEST_EXEC)
