#pragma once

class Counter {
private:
	int count;
public:
	Counter();

	void plus();

	void minus();

	int meaning();

	void setOne();

	void setNum(int num);
};