#include <set>
#include <string>
#include <vector>

using namespace std;

template <typename It>
auto MakeVector(It range_begin, It range_end) {
    return vector(range_begin, range_end);
}

int main() {
    set<string> unique_langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    auto langs = MakeVector(unique_langs.begin(), unique_langs.end());
    return 0;
}