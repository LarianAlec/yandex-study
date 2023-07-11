#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class Stack {
public:
    void Push(const Type& element) {
        this -> elements_.push_back(element);
    }

    void Pop() {
        this -> elements_.pop_back();
    }

    Type& Peek() {
        return this -> elements_.back();
    }

    const Type& Peek() const {
        return elements_.back();
    }

    void Print() const {
        PrintRange(elements_.begin(), elements_.end());
    }

    uint64_t Size() {
        return this -> elements_.size();
    }

    bool IsEmpty() {
        if (elements_.begin() == elements_.end()) {
            return true;
        }
        return false;
    }

private:
    vector<Type> elements_;
};

int main() {
    Stack<int> stack;
    for (uint32_t i = 0; i < 10; ++i) {
        stack.Push(i);
        stack.Print();
    }
    while (!stack.IsEmpty()) {
        stack.Pop();
        stack.Print();
    }
}

/*
0
0 1
0 1 2
0 1 2 3
0 1 2 3 4
0 1 2 3 4 5
0 1 2 3 4 5 6
0 1 2 3 4 5 6 7
0 1 2 3 4 5 6 7 8
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8
0 1 2 3 4 5 6 7
0 1 2 3 4 5 6
0 1 2 3 4 5
0 1 2 3 4
0 1 2 3
0 1 2
0 1
0
*/