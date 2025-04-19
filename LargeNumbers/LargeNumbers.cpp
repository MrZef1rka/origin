#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
using namespace std;

class LargeNumber {
private:
    vector<uint8_t> digits; // Младшие разряды в начале вектора

    void fromString(const string& number) {
        digits.clear();
        if (number.empty()) {
            digits.push_back(0);
            return;
        }

        for (auto it = number.rbegin(); it != number.rend(); ++it) {
            char c = *it;
            if (c < '0' || c > '9') {
                throw invalid_argument("Invalid character in number");
            }
            digits.push_back(c - '0');
        }
        trimZeros();
    }

    void trimZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

public:
    // Конструктор по умолчанию
    LargeNumber() : digits{0} {}

    // Конструктор из строки
    LargeNumber(const string& number) {
        fromString(number);
    }

    // Конструктор перемещения
    LargeNumber(LargeNumber&& other) noexcept {
        digits = move(other.digits);
    }

    // Перемещающий оператор присваивания
    LargeNumber& operator=(LargeNumber&& other) noexcept {
        if (this != &other) {
            digits = move(other.digits);
        }
        return *this;
    }

    // Оператор сложения
    LargeNumber operator+(const LargeNumber& other) const {
        LargeNumber result;
        result.digits.clear();

        const auto& a = this->digits;
        const auto& b = other.digits;

        int carry = 0;
        size_t i = 0;
        const size_t max_len = max(a.size(), b.size());

        while (i < max_len || carry) {
            int sum = carry;
            if (i < a.size()) sum += a[i];
            if (i < b.size()) sum += b[i];

            carry = sum / 10;
            result.digits.push_back(sum % 10);
            i++;
        }

        return result;
    }

    // Оператор умножения на число (long long)
    LargeNumber operator*(long long multiplier) const {
        if (multiplier < 0) {
            throw invalid_argument("Multiplier must be positive");
        }

        LargeNumber result;
        result.digits.clear();
        result.digits.resize(digits.size() + 20, 0);//+ 20 — это запас на случай, если результат умножения будет иметь больше разрядов, чем исходное число

        long long carry = 0;
        for (size_t i = 0; i < digits.size() || carry; ++i) {
            if (i < digits.size()) {
                carry += digits[i] * multiplier;
            }
            result.digits[i] = carry % 10;
            carry /= 10;
        }

        result.trimZeros();
        return result;
    }

    // Оператор умножения на другое LargeNumber
    LargeNumber operator*(const LargeNumber& other) const {
        LargeNumber result("0");

        for (size_t i = 0; i < other.digits.size(); ++i) {
            LargeNumber temp;
            temp.digits.clear();

            int carry = 0;
            for (size_t j = 0; j < digits.size() || carry; ++j) {
                int product = carry;
                if (j < digits.size()) {
                    product += digits[j] * other.digits[i];
                }
                temp.digits.push_back(product % 10);
                carry = product / 10;
            }

            temp.digits.insert(temp.digits.begin(), i, 0); // Сдвиг
            result = result + temp;
        }

        result.trimZeros();
        return result;
    }

    // Печать числа
    void print() const {
        for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
            cout << static_cast<int>(*it);
        }
    }
};

int main() {
    LargeNumber num1("999");
    LargeNumber num2("1");

    cout << "999 + 1 = ";
    (num1 + num2).print(); // 1000
    cout << endl;

    cout << "999 * 1 = ";
    (num1 * 1).print(); // 999
    cout << endl;

    cout << "123 * 45 = ";
    (LargeNumber("123") * 45).print(); // 5535
    cout << endl;

    cout << "123 * 45 (two LargeNumbers) = ";
    (LargeNumber("123") * LargeNumber("45")).print(); // 5535
    cout << endl;

    LargeNumber big("99999999999999999999999999999999999999999999999999");
    cout << "Big + 1 = ";
    (big + LargeNumber("1")).print(); // 100000000000000000000000000000000000000000000000000
    cout << endl;

    return 0;
}
