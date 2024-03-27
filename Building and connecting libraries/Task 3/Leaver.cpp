#include <iostream>
#include <string>
#include <windows.h>
#include "../DynamicLibrary/DynamicLibrary.h"


int main()
{

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string str;
    Leaver name;

    std::cout << "Введите имя: ";
    std::getline(std::cin, str);
    std::cout << name.leave(str) << std::endl;
    std::cin.get();

    return 0;
}


