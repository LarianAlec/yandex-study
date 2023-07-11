#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std::string_literals;

int main() {
    std::vector<std::string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    std::vector<std::string>::iterator it = std::find_if(langs.begin(), langs.end(), [](const std::string& lang) {
        return lang[0] == 'C';
    });
    std::cout << *it << std::endl;
}