#include <iostream>
#include <string>
#include <cassert>

using namespace std;

bool IsPalindrome(const string& text) {
    string connected_text = ""s;    
    for (const char& c : text) {
        if (c == ' ') continue;
        
        connected_text += c;
    }
    if (connected_text.empty()) return false;
    
    string reversed_text;
    for (int i = connected_text.size()-1; i >= 0; --i) {
        reversed_text += connected_text[i];
    }
    return (connected_text == reversed_text) ? true :  false;
}

void TestIsPalindrome() {
    assert(IsPalindrome(""s) == false);
    assert(IsPalindrome("              "s) == false);
    assert(IsPalindrome(" radar"s) == true);
    assert(IsPalindrome("never        odd      or         even     "s) == true);
    cout << "Testing complete!" << endl;
}

int main() {
    TestIsPalindrome();

    string text;
    getline(cin, text);

    if(IsPalindrome(text)) {
        cout << "palindrome"s << endl;
    } else {
        cout << "not a palindrome"s << endl;
    }
}

/* Классы эквивалентности:
1. Строка не пустая ("" -> not a palindrome)
2. Строка не состоит из одних пробелов ("  ", "     " -> not a palindrome)
3. Пробелы в любом количестве могут стоять между словами, а так же перед и после слов. 
("  radar    " -> palindrome)
(" never    odd or   even    " -> palindrome)
*/