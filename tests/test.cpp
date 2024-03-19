#include "testSMA.cpp"
#include "testMAC.cpp"
#include "testTrade.cpp"
#include "testLiquidity.cpp"

using namespace std;

class Test {
	public:
		Test() {
			testSMA();
			testMAC();
			testTrade();
			testLiquidity();

		}

	private:
};

int main() {
	Test test;
	return 0;
}