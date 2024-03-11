#include "Class.h"

Counter::Counter() {
	count = 0;
};
void Counter::plus() {
	++count;
};
void Counter::minus() {
	--count;
};
int Counter::meaning() {
	return count;
}
void Counter::setOne() {
	count = 1;
};
void Counter::setNum(int num) {
	count = num;
};