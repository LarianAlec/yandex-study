#include <algorithm>
#include <iostream>
#include <string>

using namespace std::string_literals;

void PrintSpacePositions(std::string& str) {
    for (auto it = find(str.begin(), str.end(), ' '); it != str.end(); it = std::find(std::next(it), str.end(), ' ')) {
        std::cout << std::distance(str.begin(), it) << " "s;
    }
}


int main() {
    std::string str = "He said: one and one and one is three"s;
    PrintSpacePositions(str);
    return 0;
}