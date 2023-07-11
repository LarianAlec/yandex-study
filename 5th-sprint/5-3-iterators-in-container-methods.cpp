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
/*template <typename It>
auto MakeSet(It range_begin, It range_end) {
    return set(range_begin, range_end);
}

template <typename It>
auto MakeVector(It range_begin, It range_end) {
    return vector(range_begin, range_end);
}*/

template <typename Container>
void EraseAndPrint(Container& container, int first_erase_index, int begin_range, int end_range) {
    container.erase(container.begin() + first_erase_index);
    PrintRange(container.begin(), container.end());
    container.erase(begin(container) + begin_range, begin(container) + end_range);
    PrintRange(container.begin(), container.end());
}

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    EraseAndPrint(langs, 2, 0, 2);
    string word = "Python"s;
    EraseAndPrint(word, 0, 0, 1);
    return 0;
}