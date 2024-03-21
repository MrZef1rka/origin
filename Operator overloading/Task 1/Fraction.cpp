#include "Fraction.h"
#include <iostream>

	Fraction::Fraction(int numerator, int denominator)
	{
		this->numerator = numerator;
		this->denominator = denominator;
		reduce();
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

	bool Fraction::operator==(Fraction other) {return (numerator == other.numerator) && (denominator == other.denominator);};
	bool Fraction::operator!=(Fraction other) { return !(*this == other); };
	bool Fraction::operator<(Fraction other) { return (numerator * other.denominator < other.numerator * denominator); };
	bool Fraction::operator>(Fraction other) { return (numerator * other.denominator > other.numerator * denominator); };
	bool Fraction::operator<=(Fraction other) { return (*this < other) && (*this == other); };
	bool Fraction::operator>=(Fraction other) { return !(*this < other) && !(*this == other); };

