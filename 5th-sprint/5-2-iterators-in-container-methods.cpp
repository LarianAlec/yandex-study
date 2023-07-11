#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Container>
void EraseAndPrint(Container& container, int index) {
    auto target_it = container.erase(container.begin() + index);
    PrintRange(container.begin(), target_it);
    PrintRange(target_it, container.end());
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    auto target_it = find (test.begin(), test.end(), 3);
    EraseAndPrint(test, 4);
    return 0;
}