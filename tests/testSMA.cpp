#include "../Classes/MovingAverage.cpp"
#include "../Classes/MovingAverageCrossover.cpp"

#ifndef TESTSMA_CPP
#define TESTSMA_CPP

void testSMA() {
	MovingAverage ma = MovingAverage(3);

	// [1]
	ma.update(1);
	assert(ma.getAverage() == 1.0);
	assert(ma.getLength() >= ma.getSize());

	// [1, 2]
	ma.update(2);
	assert(ma.getAverage() == 1.5);
	assert(ma.getLength() >= ma.getSize());

	// [1, 2, 3]
	ma.update(3);
	assert(ma.getAverage() == 2.0);
	assert(ma.getLength() >= ma.getSize());

	// [2, 3, 4]
	ma.update(4);
	assert(ma.getAverage() == 3.0);
	assert(ma.getLength() >= ma.getSize());

	// [3, 4, 5]
	ma.update(5);
	assert(ma.getAverage() == 4.0);
	assert(ma.getLength() >= ma.getSize());

	// [4, 5, 6]
	ma.update(6);
	assert(ma.getAverage() == 5.0);
	assert(ma.getLength() >= ma.getSize());

	// [5, 6, 10]
	ma.update(10);
	assert(ma.getAverage() == 7.0);
	assert(ma.getLength() >= ma.getSize());

	// [6, 10, 11]
	ma.update(11);
	assert(ma.getAverage() == 9.0);
	assert(ma.getLength() >= ma.getSize());

	deque<double> values = ma.getValues();

	cout << "PASSED: SMA update" << endl;
}

#endif