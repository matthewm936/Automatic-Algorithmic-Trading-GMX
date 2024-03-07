#include "testSMA.cpp"
#include "testMAC.cpp"
#include "testTrade.cpp"

using namespace std;

class Test {
	public:
		Test() {
			testSMA();
			testMAC();
			testTrade();

		}

	private:
};

int main() {
	Test test;
	return 0;
}