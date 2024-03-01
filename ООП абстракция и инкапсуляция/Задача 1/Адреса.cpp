#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Address {

private:
	struct AddressInfo {
		string city;
		string street;
		string houseNumber;
		string apartmentNumber;
	};

	int numAddresses = 0;
	AddressInfo* addresses;

public:

	Address() : numAddresses(0), addresses(nullptr) {};

	void Reading() {

		ifstream file("in.txt");

		file >> numAddresses;

		addresses = new AddressInfo[numAddresses];

		for (int i = 0; i < numAddresses; ++i) {

			file >> addresses[i].city >> addresses[i].street >> addresses[i].houseNumber >> addresses[i].apartmentNumber;
		}

		file.close();
	};

	void Record() {

		ofstream file2("out.txt");

		file2 << numAddresses << endl;

		cout << numAddresses << endl;

		for (int i = numAddresses - 1; i >= 0; --i) {

			file2 << addresses[i].city << ", " << addresses[i].street << ", " << addresses[i].houseNumber << ", " << addresses[i].apartmentNumber << std::endl;
			cout << addresses[i].city << ", " << addresses[i].street << ", " << addresses[i].houseNumber << ", " << addresses[i].apartmentNumber << std::endl;
		}

		file2.close();
	};

	~Address() {
		delete[] addresses;
	}
};

int main()
{
	setlocale(LC_ALL, "ru_RU.UTF-8");

	Address addresses;

	addresses.Reading();
	addresses.Record();


	return 0;
}
