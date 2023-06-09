#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

template <typename T, typename U>
void AssertEqual(const T& t, const U& u, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << "Assertation: "s << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}


void Assert(bool value, const string& hint) {
    if (value == false) {
        cout << "Assertion failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}


int main() {
    const string greetings = "Hello"s;
    //AssertEqual(5, 6, "First mast be equal second");
    Assert(greetings.empty(), "Greetings must ve non-empty"s);

    cout << "This line will not be printed"s << endl;
}