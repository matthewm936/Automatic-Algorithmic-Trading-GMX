CXX = g++

CXXFLAGS = -Wall -std=c++17

SRCS = main.cpp 

EXEC = run

$(EXEC):
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRCS)

TEST_SRCS = tests/test.cpp

TEST_EXEC = test

$(TEST_EXEC):
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(TEST_SRCS)

clean:
	rm -f $(EXEC) $(TEST_EXEC) 