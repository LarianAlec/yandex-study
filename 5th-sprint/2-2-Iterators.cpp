#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std::string_literals;

struct Lang {
    std::string name;
    int age;
};

int main() {
    std::vector<Lang> langs = {{"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37}};
    const auto& result_it = std::find_if(langs.begin(), langs.end(), [](const Lang& lang) {
        return lang.name[0] == 'J'; 
    });
    std::cout << result_it->name << ", "s << result_it->age << std::endl;
}