#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <string>
using namespace std;

template<typename T>
void print_container(const T& cont) {

	typename T::const_iterator it = cont.begin();
	// Проверяем, что контейнер не пустой
	if (it != cont.end()) {
		cout << *it;
		++it;
	}
	for (; it != cont.end(); ++it) {
	
		cout << ", " << *it ;
	}
	cout << endl;
};


int main()
{

	set<string> test_set = { "one", "two", "three", "four" };
	print_container(test_set); // four one three two. помните почему такой порядок? :)

	list<string> test_list = { "one", "two", "three", "four" };
	print_container(test_list); // one, two, three, four

	vector<string> test_vector = { "one", "two", "three", "four" };
	print_container(test_vector); // one, two, three, four


	return 0;
}

