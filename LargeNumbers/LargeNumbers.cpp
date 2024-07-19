#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
using namespace std;

class LargeNumber {
private:
    vector<long long> digits; // Вектор для хранения больших чисел, каждое число в пределах [0, 10^18)
    bool isNegative; // Флаг для хранения знака числа

    // Преобразует строку в вектор длинных чисел
    void fromString(const string& number) {
        digits.clear();
        size_t length = number.length();
        size_t pos = length;
        isNegative = (number[0] == '-');

        // Обработка отрицательных чисел
        size_t start = (isNegative) ? 1 : 0;

        while (pos > start) {
            size_t end = (pos >= 18 + start) ? pos - 18 : start;
            digits.push_back(stoll(number.substr(end, pos - end)));
            pos = end;
        }
        reverse(digits.begin(), digits.end());
    }

    // Убирает ведущие нули
    void trimLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        // Если после удаления всех ведущих нулей осталась только одна цифра равная нулю
        if (digits.size() == 1 && digits[0] == 0) {
            isNegative = false; // Ноль не имеет знака
        }
    }

public:
    // Конструктор по умолчанию
    LargeNumber() : digits(1, 0), isNegative(false) {}
    // Конструктор из строки
    LargeNumber(const string& number) {
        fromString(number);
    }
    // Конструктор копирования
    LargeNumber(const LargeNumber& other)
        : digits(other.digits), isNegative(other.isNegative) {}
    // Конструктор перемещения
    LargeNumber(LargeNumber&& other) noexcept
        : digits(move(other.digits)), isNegative(other.isNegative) {}
    // Оператор копирования
    LargeNumber& operator=(const LargeNumber& other) {
        if (this != &other) {
            digits = other.digits;
            isNegative = other.isNegative;
        }
        return *this;
    }
    // Перемещающий оператор присваивания
    LargeNumber& operator=(LargeNumber&& other) noexcept {
        if (this != &other) {
            digits = move(other.digits);
            isNegative = other.isNegative;
        }
        return *this;
    }
    // Оператор сложения двух больших чисел
    LargeNumber operator+(const LargeNumber& other) const {
        if (isNegative == other.isNegative) {
            // Сложение двух чисел с одинаковым знаком
            LargeNumber result;
            size_t size1 = digits.size();
            size_t size2 = other.digits.size();
            size_t maxSize = max(size1, size2);
            result.digits.resize(maxSize, 0);

            long long carry = 0;
            for (size_t i = 0; i < maxSize; ++i) {
                long long sum = carry;
                if (i < size1) {
                    sum += digits[i];
                }
                if (i < size2) {
                    sum += other.digits[i];
                }
                carry = sum / 1000000000000000000LL; // 10^18
                result.digits[i] = sum % 1000000000000000000LL; // 10^18
            }
            if (carry > 0) {
                result.digits.push_back(carry);
            }
            result.isNegative = isNegative;
            result.trimLeadingZeros();
            return result;
        }
        else {
            // Сложение чисел с разными знаками — выполняем вычитание
            if (*this >= other) {
                LargeNumber absOther = other;
                absOther.isNegative = !absOther.isNegative;
                return *this - absOther;
            }
            else {
                LargeNumber absThis = *this;
                absThis.isNegative = !absThis.isNegative;
                LargeNumber result = other - absThis;
                result.isNegative = !result.isNegative;
                return result;
            }
        }
    }
    // Оператор умножения на число
    LargeNumber operator*(long long multiplier) const {
        if (multiplier < 0) {
            throw invalid_argument("Multiplier cannot be negative");
        }

        LargeNumber result;
        result.digits.resize(digits.size(), 0);
        long long carry = 0;

        for (size_t i = 0; i < digits.size(); ++i) {
            long long product = digits[i] * multiplier + carry;
            carry = product / 1000000000000000000LL;
            result.digits[i] = product % 1000000000000000000LL;
        }
        if (carry > 0) {
            while (carry > 0) {
                result.digits.push_back(carry % 1000000000000000000LL);
                carry /= 1000000000000000000LL;
            }
        }
        result.isNegative = isNegative;
        result.trimLeadingZeros();
        return result;
    }
    // Метод для вывода числа
    void print() const {
        if (isNegative && !(digits.size() == 1 && digits[0] == 0)) {
            cout << "-";
        }
        if (digits.empty()) {
            cout << 0;
            return;
        }
        cout << digits.back();
        for (size_t i = digits.size() - 2; i != static_cast<size_t>(-1); --i) {
            cout << setw(18) << setfill('0') << digits[i];
        }
    }
    // Метод для сравнения по модулю
    bool operator>=(const LargeNumber& other) const {
        if (isNegative && !other.isNegative) return false;
        if (!isNegative && other.isNegative) return true;

        if (digits.size() != other.digits.size()) {
            return (digits.size() > other.digits.size()) ^ isNegative;
        }

        for (size_t i = 0; i < digits.size(); ++i) {
            if (digits[i] != other.digits[i]) {
                return (digits[i] > other.digits[i]) ^ isNegative;
            }
        }
        return true;
    }
    // Метод для вычитания двух чисел
    LargeNumber operator-(const LargeNumber& other) const {
        if (isNegative == other.isNegative) {
            // Вычитание двух чисел с одинаковым знаком
            LargeNumber result;
            size_t size1 = digits.size();
            size_t size2 = other.digits.size();
            result.digits.resize(size1, 0);

            long long borrow = 0;
            for (size_t i = 0; i < size1; ++i) {
                long long sub = digits[i] - (i < size2 ? other.digits[i] : 0) - borrow;
                if (sub < 0) {
                    sub += 1000000000000000000LL;
                    borrow = 1;
                }
                else {
                    borrow = 0;
                }
                result.digits[i] = sub;
            }
            result.isNegative = (borrow > 0);
            result.trimLeadingZeros();
            return result;
        }
        else {
            // Вычитание чисел с разными знаками — выполняем сложение
            LargeNumber absOther = other;
            absOther.isNegative = !absOther.isNegative;
            return *this + absOther;
        }
    }
};

int main() {
    LargeNumber num1("114575");
    LargeNumber num2("78524");

    LargeNumber sum = num1 + num2;
    LargeNumber mult = num1 * 2;

    cout << "Sum: ";
    sum.print();
    cout << endl;

    cout << "Product: ";
    mult.print();
    cout << endl;

    return 0;
}
