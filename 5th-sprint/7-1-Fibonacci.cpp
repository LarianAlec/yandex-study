#include <iostream>

using namespace std;

int Fibonacci(int num) {
    int result = 0;
    if (num == 0) {
        return 0;
    }
    if (num == 1) {
        return 1;
    }
    // Число Фибоначчи с порядковым номером n-1 и n-2 соотвественно
    int fib_n1 = Fibonacci(num-1);
    int fib_n2 = Fibonacci(num-2);
    result = fib_n1 + fib_n2;        
    return result;
}

/* Авторское решение
int Fibonacci(int i) {
    if (0 == i) {
        return 0;
    }
    if (1 == i) {
        return 1;
    }
    return Fibonacci(i - 1) + Fibonacci(i - 2);
}
*/

int main() {
    cout << Fibonacci(8) << endl;
}