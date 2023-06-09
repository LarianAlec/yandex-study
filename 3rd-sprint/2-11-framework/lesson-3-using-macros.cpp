#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;


template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                    const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << ".";
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}



void AssertImpl(const bool value, const string& value_str, const string& file,
                const string& func, unsigned line, const string& hint) {
    if (value == false) {
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT("s << value_str << ") failed."s ;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT(expr) AssertImpl((expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_HINT(expr, hint) AssertImpl((expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

int main() {
    const string greetings = "Hello"s;
    //ASSERT_EQUAL_HINT(5, 6, "First mast be equal second"s);
    ASSERT_HINT(greetings.empty(), "Greetings must ve non-empty"s);

    cout << "This line will not be printed"s << endl;
}