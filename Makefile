# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17

# Test source files
TEST_SRCS = tests/test.cpp Classes/Trade.cpp Classes/MovingAverage.cpp Classes/Portfolio.cpp

# Test executable name
TEST_EXEC = test

$(TEST_EXEC):
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(TEST_SRCS)

clean:
	rm -f $(TEST_EXEC)