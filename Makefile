CXX = g++
CXXFLAGS = -Wall -std=c++17
SRCS = main.cpp 
EXEC = run
# TEST_SRCS = tests/test.cpp
# TEST_EXEC = test

$(EXEC): $(SRCS)
	@echo "Compiling $@..."
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilation complete."

# $(TEST_EXEC): $(TEST_SRCS)
# 	@echo "Compiling $@..."
# 	@$(CXX) $(CXXFLAGS) -o $@ $^
# 	@echo "Compilation complete."

clean:
	rm -f $(EXEC) $(TEST_EXEC)
