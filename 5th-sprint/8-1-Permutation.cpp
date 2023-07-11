#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

// функция, записывающая элементы диапазона в строку
template <typename It>
string PrintRangeToString(It range_begin, It range_end) {
    ostringstream out;
    for (auto it = range_begin; it != range_end; ++it) {
        out << *it << " "s;
    }
    out << endl;
    // получаем доступ к строке с помощью метода str для ostringstream
    return out.str();
}

template <typename It>
vector<string> GetPermutations(It range_begin, It range_end) {
    vector<string> permutations; // перестановки
    sort(range_begin, range_end);
    reverse(range_begin, range_end);
    do {
        permutations.push_back(PrintRangeToString(range_begin, range_end));
    } while (prev_permutation(range_begin, range_end));
    return permutations;
}

int main() {
    vector<int> permutation = {1 , 7 , 9 , 5 , 4 , 3 , 2 , 6};
    //iota(permutation.begin(), permutation.end(), 1);
    auto result = GetPermutations(permutation.begin(), permutation.end());
    for (const auto& s : result) {
        cout << s;
    }
    return 0;
}