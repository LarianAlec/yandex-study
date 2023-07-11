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
/*
template <typename It>
auto MakeSet(It range_begin, It range_end) {
    return set(range_begin, range_end);
}

template <typename It>
auto MakeVector(It range_begin, It range_end) {
    return vector(range_begin, range_end);
}*/

template <typename Container, typename It>
void EraseAndPrint(Container& container, It target) {
    auto target_position = container.erase(target);
    PrintRange(container.begin(), target_position);
    PrintRange(target_position, container.end());
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    auto target_it = find (test.begin(), test.end(), 3);
    EraseAndPrint(test, target_it);
    return 0;
}