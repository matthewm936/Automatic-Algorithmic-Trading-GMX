#ifndef MOVINGAVERAGE_CPP
#define MOVINGAVERAGE_CPP

#include <deque>
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

class MovingAverage {
private:
	int length;
	int size;

	double average;
	double sum;

	deque<double> values;

public:
	string speed;

	MovingAverage(int length) {
		this->length = length;
		size = 0;
		average = 0.0;
		sum = 0.0;

		values = deque<double>();
	}

	void update(double currentPrice) {
		values.push_front(currentPrice);
		size++;
		sum += currentPrice; 

		if (size > length) {
			sum -= values.back();
			values.pop_back();
			size--;
		}

		average = sum / size;
	}

	double getAverage() {
		return average;
	}

	deque<double> getValues() {
		return values;
	}

	const int getLength() {
		return length;
	}

	const int getSize() {
		return size;
	}

	void printValues() {
		cout << "Values: ";
		for (double value : values) {
			cout << value << " ";
		}
		cout << endl;
	}

	void printState() {
		string state = this->speed + " Length: " + to_string(length) + " Size: " + to_string(size) + " Average: " + to_string(average) + " Sum: " + to_string(sum);
		cout << state << endl;
		printValues();
	}
};

#endif 
