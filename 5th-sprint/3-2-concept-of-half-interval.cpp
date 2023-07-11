#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

template <typename It>
void PrintRange(It range_begin, const It range_end) {
    if (range_begin == range_end) return;    
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Container, typename Element>
void FindAndPrint(const Container& container, const Element& element_to_find) {
    auto it_begin = begin(container);
    auto it_end = end(container);
    auto it = find(it_begin, it_end, element_to_find);
    PrintRange(it_begin, it);
    PrintRange(it, it_end);
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    cout << "Test1"s << endl;
    FindAndPrint(test, 3);
    cout << "Test2"s << endl;
    FindAndPrint(test, 0); // элемента 0 нет в контейнере
    cout << "End of tests"s << endl;

    set<int>::iterator it = test.end();
    cout << *it << endl;
}