#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Address {

private:
	struct AddressInfo {
		string city;
		string street;
		int houseNumber = 0;
		int apartmentNumber = 0;
	};

	int numAddresses;
	AddressInfo* addresses;

public:

	Address() {

		numAddresses = 0;
		addresses = nullptr;
	};

	void sortAddresses() {
		for (int i = 0; i < numAddresses - 1; ++i) {
			for (int j = 0; j < numAddresses - i - 1; ++j) {
				if (addresses[j].city > addresses[j + 1].city) {
					swap(addresses[j], addresses[j + 1]);
				}
			}
		}
	}

	void Reading() {

		ifstream file("in.txt");

		file >> numAddresses;

		addresses = new AddressInfo[numAddresses];
		
		for (int i = 0; i < numAddresses; ++i) {

			file >> addresses[i].city >> addresses[i].street >> addresses[i].houseNumber >> addresses[i].apartmentNumber;
		}

		file.close();
	};

	void Record() const {

		ofstream file2("out.txt");

		file2 << numAddresses << endl;

		cout << numAddresses << endl;

		for (int i = 0; i < numAddresses; i++) {

			file2 << addresses[i].city << ", " << addresses[i].street << ", " << addresses[i].houseNumber << ", " << addresses[i].apartmentNumber << std::endl;
			cout << addresses[i].city << ", " << addresses[i].street << ", " << addresses[i].houseNumber << ", " << addresses[i].apartmentNumber << std::endl;
		}

		file2.close();
		delete[] addresses;
	};

};

int main()
{
	setlocale(LC_ALL, "ru_RU.UTF-8");

	Address addresses;

	addresses.Reading();

	addresses.sortAddresses();

	addresses.Record();


	return 0;
}
