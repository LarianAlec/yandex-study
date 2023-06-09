#include <iostream> //добавлено
#include <sstream> // добавлено

#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};


//template<typename A, typename B> auto Predicat(A, B, A);

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    }

    template< typename DocumentPredicat >
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicat document_predicat) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, document_predicat);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    vector<Document> FindTopDocuments(const string& raw_query) const{
        return FindTopDocuments(raw_query, [](int document_id, DocumentStatus status,  int rating) {
            return status==DocumentStatus::ACTUAL;
        });
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(raw_query, [status](int document_id, DocumentStatus status_predicat, int rating) {
            return status_predicat == status;
        });
    }


    int GetDocumentCount() const {
        return documents_.size();
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query,
                                                        int document_id) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, documents_.at(document_id).status};
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }
    template< typename DocumentPredicat >
    vector<Document> FindAllDocuments(const Query& query,  DocumentPredicat document_predicat) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicat(document_id, 
                    document_data.status, 
                    document_data.rating )) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back(
                {document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }

}

// Добавление документов. Добавленный документ должен находиться по поисковому
// запросу, который содержит слова из документа
void TestAddDocumentsAndFindDocument() {
    const int doc_id1 = 42;
    const int doc_id2 = 4;
    const string content1 = "cat in the city"s;
    const string content2 = "dog near bus station in the city"s;

    const vector<int> ratings = {1, 2, 3};
    // Перед добавлением документа список документов пуст. После добавления
    // документа список документов увеличивается в размере
    {
        SearchServer server;
        assert(server.GetDocumentCount() == 0);
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings);
        assert(server.GetDocumentCount() == 1);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings);
        assert(server.GetDocumentCount() == 2);
    }

    //При добавлении нескольких документов с одинаковым id  список документов увеличиваться не должен
    {
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id1, content2, DocumentStatus::ACTUAL, ratings);
        assert(server.GetDocumentCount() == 1);
    }

    // Добавленный документ должен находиться по поисковому запросу
    {
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings);
        const auto found_docs1 = server.FindTopDocuments("cat"s);
        assert(found_docs1.size() == 1);
        const Document& first_doc1 = found_docs1[0];
        assert(first_doc1.id == doc_id1);
   
        // если в поисковом запросе присутсвует слово, встречающееся
        // в нескольких документах, выдаются несколько документов, 
        // причем на первом месте находится документ с наибольшим 
        // пересеченеием слов
        const auto found_docs2 = server.FindTopDocuments("dog in the city"s);
        assert(found_docs2.size() == 2);
        const Document& first_doc2 = found_docs2[0];
        assert(first_doc2.id == doc_id2);
    }

}


void TestMinusWordsInQueryExludeDocumentFromResult() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    const string raw_query = "cat -city"s;

    // Документы, содержащие минус-слова поискового запроса, не должны
    // включаться в результаты поиска.
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto find_docs = server.FindTopDocuments(raw_query);
        assert(find_docs.empty());
    }
}

 
void TestMatchingDocuments() {
    const int doc_id = 42;
    const string content = "fluffy cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    // При матчинге документа по поисковому запросу должны быть возвращены все слова
    // из поискового запроса, присутствующие в документе.
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto [words, status] = server.MatchDocument("fluffy cat", doc_id);
        assert(words.size() == 2);
    }
    // Если есть соответствие хотя бы по одному минус-слову, должен возвращаться пустой список слов.
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const vector<string> words = get<vector<string>>(server.MatchDocument("fluffy cat -city", doc_id));
        assert(words.size() == 0);
    }
}

void TestSortByRelevance() {
    //doc1
    const int doc_id1 = 42;
    const string content1 = "fluffy cat in the city"s;
    //doc2
    const int doc_id2 = 4;
    const string content2 = "black dog near bus station in the city";
    //doc3
    const int doc_id3 = 12;
    const string content3 = "black fluffy cat in the city";
    const vector<int> ratings = {1, 2, 3};

    const string query = "black fluffy cat in the city";
    // Возвращаемые при поиске документов результаты должны быть 
    // отсортированы в порядке убывания релевантности.
    {
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id3, content3, DocumentStatus::ACTUAL, ratings);
        const auto documents = server.FindTopDocuments(query);

        //самый релевантный документ должен быть doc3
        assert(documents[0].id == doc_id3);
        assert(documents[1].id == doc_id1);
        assert(documents[2].id == doc_id2);
    }
}

void TestRatingIsEqualArithmeticMeanOfTheDocumentsRatings() {
    const int doc_id = 42;
    const string content = "fluffy cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const Document& document = server.FindTopDocuments("fluffy cat in the city")[0];

        // вычисление среднего арифметического
        int sum = 0;
        for (int i = 0; i < static_cast<int>(ratings.size()); ++i) {
            sum += ratings[i];
        }
        const int arithmetic_mean = sum/static_cast<int>(ratings.size());

        assert(document.rating == arithmetic_mean);
    }
}

void TestPredicatFiltrating() {
    const vector<int> rating = {1, 2, 3};
    const string content = "cat in the city"s;
    const string query = "cat in the city"s;

    // с помощью предиката должен реализован поиск по статусу
    {
        SearchServer server;
        assert(0 == server.FindTopDocuments(query, DocumentStatus::ACTUAL).size()*1);
        assert(0 == server.FindTopDocuments(query, DocumentStatus::BANNED).size()*1);
        assert(0 == server.FindTopDocuments(query, DocumentStatus::IRRELEVANT).size()*1);
        assert(0 == server.FindTopDocuments(query, DocumentStatus::REMOVED).size()*1);

        server.AddDocument(1, content, DocumentStatus::ACTUAL, rating);
        server.AddDocument(2, content, DocumentStatus::IRRELEVANT, rating);
        server.AddDocument(3, content, DocumentStatus::BANNED, rating);
        server.AddDocument(4, content, DocumentStatus::REMOVED, rating);

        assert(1 == server.FindTopDocuments(query, DocumentStatus::ACTUAL).size()*1);
        assert(1 == server.FindTopDocuments(query, DocumentStatus::BANNED).size()*1);
        assert(1 == server.FindTopDocuments(query, DocumentStatus::IRRELEVANT).size()*1);
        assert(1 == server.FindTopDocuments(query, DocumentStatus::REMOVED).size()*1);
    }

    //с помощью предиката должен быть реализован поиск по лямбда-функции
    {  
        SearchServer server;
        server.AddDocument(1, content, DocumentStatus::ACTUAL, rating);
        server.AddDocument(2, content, DocumentStatus::IRRELEVANT, rating);
        server.AddDocument(3, content, DocumentStatus::BANNED, rating);
        server.AddDocument(4, content, DocumentStatus::REMOVED, rating);

        vector<Document> even_docs = server.FindTopDocuments(query, [](int document_id, DocumentStatus status, int rating) {
            return document_id % 2 == 0;
        });
        assert(even_docs.size() == 2);
    }
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    TestAddDocumentsAndFindDocument();
    TestMinusWordsInQueryExludeDocumentFromResult();
    TestMatchingDocuments();
    TestSortByRelevance();
    TestRatingIsEqualArithmeticMeanOfTheDocumentsRatings();
    TestPredicatFiltrating();
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}