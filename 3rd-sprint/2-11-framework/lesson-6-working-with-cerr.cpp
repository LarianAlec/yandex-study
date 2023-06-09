#include <iostream>
#include <string>

using namespace std;

template <typename TestFunc>
void RunTestImpl(TestFunc test, const string& func_name) {
    test();
    cerr << func_name << " OK"s << endl;
}

#define RUN_TEST(func) RunTestImpl(func, #func)

void Test1() {
    //...
}

int main() {
    RUN_TEST(Test1);
}