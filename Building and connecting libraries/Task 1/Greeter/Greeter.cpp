#include <iostream>
#include <string>
#include <windows.h>
#include "../StaticLibrary/StaticLibrary.h"


int main()
{

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string str;
    Greeter name;

    std::cout << "Введите имя: ";
    std::getline(std::cin, str);
    std::cout << name.greet(str) << std::endl;
    std::cin.get();

    return 0;
}

