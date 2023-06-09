#include <numeric>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator) {
        numerator_ = numerator;
        denominator_ = 1;
    }

    Rational(int numerator, int denominator) {
        if (denominator < 0) {
            denominator *= (-1);
            numerator *= (-1);
        }
        int greatest_common_divisor = gcd(numerator, denominator);
        numerator_ = numerator/greatest_common_divisor;
        denominator_ = denominator/greatest_common_divisor;
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
};