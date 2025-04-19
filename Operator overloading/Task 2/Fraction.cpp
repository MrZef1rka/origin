#include "Fraction.h"
#include <iostream>
#include <algorithm>

Fraction::Fraction(int numerator, int denominator) {
    this->numerator = numerator;
    this->denominator = denominator;
    reduce();
}

int Fraction::findLCM(int a, int b) {
    int lcm = std::max(a, b);
    while (true) {
        if (lcm % a == 0 && lcm % b == 0) {
            break;
        }
        ++lcm;
    }
    return lcm;
}

int Fraction::getcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void Fraction::reduce() {
    int gcd = getcd(std::abs(numerator), std::abs(denominator));
    numerator /= gcd;
    denominator /= gcd;

    if (denominator < 0) {
        numerator *= -1;
        denominator = std::abs(denominator);
    }
}

Fraction Fraction::operator+(Fraction other) {
    int lcm = findLCM(denominator, other.denominator);
    int new_numerator = (numerator * (lcm / denominator)) + (other.numerator * (lcm / other.denominator));
    return Fraction(new_numerator, lcm);
}

Fraction Fraction::operator-(Fraction other) {
    int lcm = findLCM(denominator, other.denominator);
    int new_numerator = (numerator * (lcm / denominator)) - (other.numerator * (lcm / other.denominator));
    return Fraction(new_numerator, lcm);
}

Fraction Fraction::operator*(Fraction other) {
    int new_numerator = (numerator * other.numerator);
    int new_denominator = (denominator * other.denominator);
    return Fraction(new_numerator, new_denominator);
}

Fraction Fraction::operator/(Fraction other) {
    int new_numerator = (numerator * other.denominator);
    int new_denominator = (denominator * other.numerator);
    return Fraction(new_numerator, new_denominator);
}

Fraction Fraction::operator++() {
    numerator += denominator;
    return *this;
}

Fraction Fraction::operator++(int) {
    Fraction temp(*this);
    ++(*this);
    return temp;
}

Fraction Fraction::operator--() {
    numerator -= denominator;
    return *this;
}

Fraction Fraction::operator--(int) {
    Fraction temp(*this);
    --(*this);
    return temp;
}

Fraction Fraction::operator-() const {
    return Fraction(-numerator, denominator);
}

std::ostream& operator<<(std::ostream& out, const Fraction& fraction) {
    out << fraction.numerator << "/" << fraction.denominator;
    return out;
}
