#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <cassert>
#include <iomanip>
#include <cstdlib>

using namespace std;

template <typename Container>
ostream& Print(ostream& out, const Container& container) {
    bool is_first = true;
    for (const auto& element : container) {
        if (!is_first) {
            out << ", ";
        }
        is_first = false;
        out << element;
    }
    return out;
}

template <typename Key, typename Value>
ostream& Print(ostream& out, const map<Key, Value>& map_container) {
    bool is_first = true;
    for (const auto& [key, value] : map_container) {
        if (!is_first) {
            out << ", ";
        }
        is_first = false;
        out << key << ": " << value;
    }
    return out;
} 

template <typename RandomType>
ostream& operator<<(ostream& out, const vector<RandomType>& container) {
    out << "["s;
    Print(out, container);
    out << "]"s;
    return out;
}

template <typename RandomType>
ostream& operator<<(ostream& out, const set<RandomType>& container) {
    out << "{"s;
    Print(out, container);
    out << "}"s;
    return out;
}

template <typename Key, typename Value>
ostream& operator<<(ostream& out, map<Key, Value> map_container) {
    out << "{"s;
    Print(out, map_container);
    out << "}"s;
    return out;
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

class Synonyms {
public:
    void Add(const string& first_word, const string& second_word) {
        synonyms_[first_word].insert(second_word);
        synonyms_[second_word].insert(first_word);
    }

    size_t GetSynonymCount(const string& word) const {
        if(synonyms_.count(word) != 0) {
            return synonyms_.at(word).size();
        }
        return 0;
    }

    bool AreSynonyms(const string& first_word, const string& second_word) const {
        if (synonyms_.count(first_word) != 0) {
            if (synonyms_.at(first_word).count(second_word) == 1) {
                return true;
            } else {
                return false;
            }    
       }
       return false;
    }

private:
    map<string, set<string>> synonyms_;
};

void TestAddingSynonymsIncreasesTheirCount() {
    Synonyms synonyms;
    ASSERT(synonyms.GetSynonymCount("music"s) == 0);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    // Два слова являются синонимами друг друга
    ASSERT(synonyms.GetSynonymCount("music"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);

    synonyms.Add("music"s, "tune"s);
    ASSERT(synonyms.GetSynonymCount("music"s) == 2);

    // Отношение синонимичности в нашей постановке задачи не является транзитивным.
    // Поэтому слова "tune" и "melody" синонимами друг друга не являются,
    // несмотря на то что оба они являются синонимами слова "music".

    ASSERT(synonyms.GetSynonymCount("tune"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);
}

void TestAreSynonyms() {
    Synonyms synonyms;
    //пока слова не добалены в словарь, мы должны получать ЛОЖЬ
    ASSERT(synonyms.AreSynonyms("skateboard"s , "board"s) == false);

    synonyms.Add("skateboard"s, "board"s);
    ASSERT(synonyms.AreSynonyms("skateboard"s , "board"s) == true);
    ASSERT(synonyms.AreSynonyms("board"s, "skateboard"s) == true);
}

void TestSynonyms() {
    TestAddingSynonymsIncreasesTheirCount();
    TestAreSynonyms();
}


int main() {
    TestSynonyms();
    Synonyms synonyms;

    string line;
    // В случае ошибок чтения из cin произойдёт выход из цикла
    while (getline(cin, line)) {
        istringstream command(line);
        string action;
        command >> action;

        if (action == "ADD"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            synonyms.Add(first_word, second_word);
        } else if (action == "COUNT"s) {
            string word;
            command >> word;
            synonyms.GetSynonymCount(word);
        } else if (action == "CHECK"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            if (synonyms.AreSynonyms(first_word, second_word)){
                cout << "YES"s << endl;
            } else {
                cout << "NO"s << endl;
            }
        } else if (action == "EXIT"s) {
            break;
        }
    }
}