#include <iostream>
#include <map>
using namespace std;


static map<char, int> calculateFrequencies(const string& str) {

	map<char, int> frequency_map;

	for (char ch : str) {
	
		frequency_map[ch]++;
	}
	
	return frequency_map;
}

int main()
{
	
	string input = "Hello world!!";

	map<char, int> frequency = calculateFrequencies(input);

	for (const auto& pair : frequency) {

		cout << pair.first << ": " << pair.second << endl;
	}

    return 0;
}

