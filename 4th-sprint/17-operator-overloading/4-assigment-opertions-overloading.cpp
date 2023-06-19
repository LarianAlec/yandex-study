#include <algorithm>
#include <numeric>
#include <iostream>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        Normalize();
    }

    Rational& operator+=(Rational right) {
        numerator_ = Numerator()*right.Denominator() + right.Numerator()* Denominator();
        denominator_ = Denominator() * right.Denominator();
        Normalize();
        return *this;
    }

    Rational& operator-=(Rational right) {
        numerator_ = Numerator()*right.Denominator() - right.Numerator()* Denominator();
        denominator_ = Denominator() * right.Denominator();
        Normalize();
        return *this;
    }

    Rational& operator*=(Rational right) {
        numerator_ = Numerator() * right.Numerator();
        denominator_ = Denominator() * right.Denominator();
        Normalize();
        return *this;
    }    

    Rational& operator/=(Rational right) {
        numerator_ = Numerator() * right.Denominator();
        denominator_ = Denominator() * right.Numerator();
        Normalize();
        return *this;
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = std::__gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& output, const Rational& rational) {
    output << rational.Numerator() << '/' << rational.Denominator();
    return output; 
}

istream& operator>>(istream& input, Rational& rational) {
    int numerator, denominator;
    char slash;
    input >> numerator >> slash >> denominator;
    rational = Rational{numerator, denominator};
    return input;
}

Rational operator+(Rational left, Rational right) {
    return left += right;
}

Rational operator-(Rational left, Rational right) {
    return left -= right;
}

Rational operator*(Rational left, Rational right) {
    return left *= right;
}

Rational operator/(Rational left, Rational right) {
    return left /= right;
}

Rational operator+(Rational rational) {
    return rational;
}

Rational operator-(Rational rational) {
    const int numerator = -rational.Numerator();
    const int denominator = rational.Denominator();
    return {numerator, denominator};
}

int main() {
    Rational rational;
    cout << "Введите дробь: "s;
    cin >> rational;

    Rational num;
    cout << "Введите целое число: "s;
    cin >> num;

    cout << "Сложение дробей: "s << rational + num << endl;
    cout << "Вычитание дробей: "s << rational - num << endl;
}