#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <cassert>

using namespace std;


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
    assert(synonyms.GetSynonymCount("music"s) == 0);
    assert(synonyms.GetSynonymCount("melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    // Два слова являются синонимами друг друга
    assert(synonyms.GetSynonymCount("music"s) == 1);
    assert(synonyms.GetSynonymCount("melody"s) == 1);

    synonyms.Add("music"s, "tune"s);
    assert(synonyms.GetSynonymCount("music"s) == 2);

    // Отношение синонимичности в нашей постановке задачи не является транзитивным.
    // Поэтому слова "tune" и "melody" синонимами друг друга не являются,
    // несмотря на то что оба они являются синонимами слова "music".

    assert(synonyms.GetSynonymCount("tune"s) == 1);
    assert(synonyms.GetSynonymCount("melody"s) == 1);
}

void TestAreSynonyms() {
    Synonyms synonyms;
    //пока слова не добалены в словарь, мы должны получать ЛОЖЬ
    assert(synonyms.AreSynonyms("skateboard"s , "board"s) == false);

    synonyms.Add("skateboard"s, "board"s);
    assert(synonyms.AreSynonyms("skateboard"s , "board"s) == true);
    assert(synonyms.AreSynonyms("board"s, "skateboard"s) == true);
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