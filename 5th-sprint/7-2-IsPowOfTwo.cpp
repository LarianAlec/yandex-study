#include <iostream>

using namespace std;

bool IsPowOfTwo(int num) {
    if (num == 1) {
        return true;
    }
    if (num == 0) {
        return false;
    }
    if (num % 2 == 0) {
        return IsPowOfTwo(num/2);
    } 
    return false;
}

int main() {
    cout << IsPowOfTwo(1024) << endl; //expected 1 -- true
}