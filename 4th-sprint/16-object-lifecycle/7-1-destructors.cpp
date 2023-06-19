#include <iostream>
#include <string>

using namespace std;

class Greeter {
public:
    Greeter(const string& name1) 
        :name1_(name1)
        {
            cout << "Hello, "s << name1_ << "!"s << endl;
        }

    Greeter(const string& name1, const string& name2) : Greeter(name1) 
    {
        name2_ = name2;
        cout << "Hello, "s << name2_ <<"!"s << endl;

    }

    ~Greeter() {
        // Поля класса внутри деструктора еще не уничтожены, и к ним можно обращаться
        cout << "Goodbye, "s << name1_ << "and" << name2_ << "!"s << endl;
    }
private:
    string name1_;
    string name2_;
};

Greeter ReturnGreeter() {
    return Greeter{"result1"s, "result2"};
}


int main() {
    Greeter var1("var1"s);
    Greeter var2("var2-1", "var2-2");
}