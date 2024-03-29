#include <algorithm>    // random_shuffle c++14
#include <iostream> 
#include <numeric>      // iota
#include <vector>

template <typename It>
void PrintRange(It begin_range, It end_range) {
    for (auto it = begin_range; it != end_range; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template<typename Iter>
void Merge(Iter beg, Iter mid, Iter end)
{
    std::vector<typename Iter::value_type> temp;
    temp.reserve(std::distance(beg, end));

    Iter left = beg;
    Iter right = mid;
    while (left != mid and right != end) {
        if (*right < *left) {
            temp.emplace_back(*right++);
        }
        else {
            temp.emplace_back(*left++);
        }
    }
    temp.insert(temp.end(), left, mid);
    temp.insert(temp.end(), right, end);

    std::move(temp.begin(), temp.end(), beg);
}

template<typename Iter>
void MergeSort(Iter beg, Iter end)
{
    int size = std::distance(beg, end);
    if (size <= 1) {
        return;
    }
    auto mid = std::next(beg, size / 2);
    // sort left half
    MergeSort(beg, mid);
    // sort right half
    MergeSort(mid, end);
    // merge halves
    Merge(beg, mid, end);
}


int main() {
    std::vector<int> test_vector(10);
    std::iota(test_vector.begin(), test_vector.end(), 1);
    std::random_shuffle(test_vector.begin(), test_vector.end());
    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());
    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());
    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());
    return 0;
}

// Авторское решение 
/*
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // 1. Если диапазон содержит меньше 2 элементов, выходим из функции
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    // 2. Создаем вектор, содержащий все элементы текущего диапазона
    vector<typename RandomIt::value_type> elements(range_begin, range_end);

    // 3. Разбиваем вектор на две равные части
    auto mid = elements.begin() + range_length / 2;

    // 4. Вызываем функцию MergeSort от каждой половины вектора
    MergeSort(elements.begin(), mid);
    MergeSort(mid, elements.end());

    // 5. С помощью алгоритма merge сливаем отсортированные половины
    // в исходный диапазон
    // merge -> http://ru.cppreference.com/w/cpp/algorithm/merge
    merge(elements.begin(), mid, mid, elements.end(), range_begin);
}

int main() {
    vector<int> test_vector(10);

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    iota(test_vector.begin(), test_vector.end(), 1);

    // random_shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    random_shuffle(test_vector.begin(), test_vector.end());

    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());

    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());

    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());

    return 0;
}
*/