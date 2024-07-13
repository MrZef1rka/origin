#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

static map<char, int> calculateFrequencies(const string& str) {
	map<char, int> frequency_map;
	for (char ch : str) {

		frequency_map[ch]++;
	}
	return frequency_map;
};

int main() {
	string input = "Hello world!!";
	map<char, int> frequency = calculateFrequencies(input);
	vector<pair<char, int>> frequency_vector(frequency.begin(), frequency.end());

	sort(frequency_vector.begin(), frequency_vector.end(),
		[](const pair<char, int>& a, const pair<char, int>& b) {
			return a.second > b.second;
		});

	for (const auto& pair : frequency_vector) {

		cout << pair.first << ": " << pair.second << endl;
	}
	return 0;
};

