#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
using namespace std;

// Базовый класс наблюдателя
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const string& message) {}
    virtual void onError(const string& message) {}
    virtual void onFatalError(const string& message) {}
};

// Наблюдаемый класс
class Subject {
    list<Observer*> observers_; 

    // Удаляем "битые" указатели (на разрушенные объекты)
    void cleanup() {
        observers_.remove(nullptr);
    }

public:
    // Добавление наблюдателя
    void addObserver(Observer* observer) {
        observers_.push_back(observer);
    }

    // Удаление наблюдателя
    void removeObserver(Observer* observer) {
        auto it = find(observers_.begin(), observers_.end(), observer);
        if (it != observers_.end()) {
            *it = nullptr;
        }
        cleanup();
    }

    // Методы событий
    void warning(const string& message) const {
        for (auto* observer : observers_) {
            if (observer) {
                observer->onWarning(message);
            }
        }
    }

    void error(const string& message) const {
        for (auto* observer : observers_) {
            if (observer) {
                observer->onError(message);
            }
        }
    }

    void fatalError(const string& message) const {
        for (auto* observer : observers_) {
            if (observer) {
                observer->onFatalError(message);
            }
        }
    }
};

// Наблюдатель для предупреждений (в консоль)
class WarningObserver : public Observer {
public:
    void onWarning(const string& message) override {
        cout << "[WARNING] " << message << endl;
    }
};

// Наблюдатель для ошибок (в файл)
class ErrorObserver : public Observer {
    string filePath_;
public:
    ErrorObserver(const string& filePath) : filePath_(filePath) {}

    void onError(const string& message) override {
        ofstream file(filePath_, ios::app);
        if (file.is_open()) {
            file << "[ERROR] " << message << endl;
        }
    }
};

// Наблюдатель для фатальных ошибок (в консоль и файл)
class FatalErrorObserver : public Observer {
    string filePath_;
public:
    FatalErrorObserver(const string& filePath) : filePath_(filePath) {}

    void onFatalError(const string& message) override {
        cout << "[FATAL ERROR] " << message << endl;
        
        ofstream file(filePath_, ios::app);
        if (file.is_open()) {
            file << "[FATAL ERROR] " << message << endl;
        }
    }
};

int main() {
    // Создаём наблюдаемый объект
    Subject subject;

    // Создаём наблюдателей
    WarningObserver warningObserver;
    ErrorObserver errorObserver("errors.log");
    FatalErrorObserver fatalErrorObserver("fatal_errors.log");

    // Подписываем наблюдателей
    subject.addObserver(&warningObserver);
    subject.addObserver(&errorObserver);
    subject.addObserver(&fatalErrorObserver);

    // Генерируем события
    subject.warning("Disk space low");
    subject.error("Invalid input");
    subject.fatalError("Server crashed");

    // Отписываем один из наблюдателей
    subject.removeObserver(&errorObserver);
    subject.error("This error will not be logged");  // Не попадёт в файл

    return 0;
}