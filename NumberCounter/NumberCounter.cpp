#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;


class Functor {
private:
	int sum;
	int count;
public:
	Functor() : sum(0), count(0) {}


	void operator()(int x) {
		if (x % 3 == 0) {
			sum += x;
			count++;
		}

	}

	int get_sum() const {

		return sum;
	}

	int get_count() const {

		return count;
	}
};



int main()
{

	vector<int> in = { 5, 4, 8, 15, 45, 10 };

	Functor fun;


	cout << "[IN]: ";

	for (int num : in) {
		cout << num << " ";
	}
	cout << endl;

	for_each(in.begin(), in.end(), ref(fun));

	cout << "[OUT]: get_sum() = " << fun.get_sum() << endl;
	cout << "[OUT]: get_count() = " << fun.get_count();

	return 0;
}
