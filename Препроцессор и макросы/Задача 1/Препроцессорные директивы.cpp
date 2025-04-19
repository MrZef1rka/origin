#include <iostream>
using namespace std;

#define MODE 1

int add(int a, int b) {
    return a + b;
}

int main() {
    setlocale(LC_ALL, "ru");

    int a = 0;
    int b = 0;

#ifdef MODE

#if MODE == 0
    cout << "Работаю в режиме тренировки." << endl;
#elif MODE == 1
    cout << "Работаю в боевом режиме." << endl;
    cout << "Первое число: ";
    cin >> a;
    cout << "Второе число: ";
    cin >> b;
    cout << "Сумма чисел: " << add(a, b);
    return 0;
#else
    cout << "Неизвестный режим. Завершение работы." << endl;
#endif
#else
#error Необходимо определить значение MODE
#endif
}
