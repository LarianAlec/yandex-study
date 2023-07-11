#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class StackMin {
public:
    void Push(const Type& element) {
        if(IsFirstAdd()) {
            min_element_ = element;
        }
        if (element < min_element_) {
            min_element_ = element;
        }
        elements_.push_back(element);
        min_elements_.push_back(min_element_);
    }

    void Pop() {
        min_elements_.pop_back();
        elements_.pop_back();
    }

    void Print() const {
        PrintRange(elements_.begin(), elements_.end());
    }

    bool IsEmpty() const {
        if (elements_.begin() == elements_.end()) {
            return true;
        }
        return false;
    }

    Type Peek() {
        return elements_.back();
    }

    const Type Peek() const {
        return elements_.back();
    }

     Type PeekMin() {
        return min_elements_.back();
    }

    const Type PeekMin() const {
        return min_elements_.back();
    }


private:
    vector<Type> elements_;
    vector<Type> min_elements_;
    Type min_element_;

    bool IsFirstAdd() {
        if(elements_.begin() == elements_.end()) {
            return true;
        }
        return false;
    }
};

int main() {
    StackMin<int> stack;
    vector<int> values(5);
    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_shuffle(values.begin(), values.end());
    // заполняем стек
    for (int i = 0; i < 5; ++i) {
        stack.Push(values[i]);
    }
    // печатаем стек и его минимум, постепенно убирая из стека элементы
    while (!stack.IsEmpty()) {
        stack.Print();
        cout << "Минимум = "s << stack.PeekMin() << endl;
        stack.Pop();
    }
}