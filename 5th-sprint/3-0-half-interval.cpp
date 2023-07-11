#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Lang {
    string name;
    int age;
};

int main() {
    vector<Lang> langs = {{"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37}};

    auto result = find_if(langs.begin(), langs.end(), [](const Lang &lang) {
        return lang.name[0] == 'K';
    });
    cout << result->name << ", "s << result->age << endl;
}