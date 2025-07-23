#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

// Типы сообщений
enum class Type {
    WARNING,
    ERROR,
    FATAL_ERROR,
    UNKNOWN
};

// Сообщение для логирования
class LogMessage {
    Type type_;
    string message_;
public:
    LogMessage(Type type, const string& message) 
        : type_(type), message_(message) {}

    Type type() const { return type_; }
    const string& message() const { return message_; }
};

class Handler {
protected:
    Handler* nextHandler_;  // Следующий обработчик в цепочке
public:
    Handler() : nextHandler_(nullptr) {}

    // Установка следующего обработчика
    void setNext(Handler* next) {
        nextHandler_ = next;
    }

    // Обработка сообщения (виртуальный метод)
    virtual void handle(const LogMessage& msg) {
        if (nextHandler_) {
            nextHandler_->handle(msg);
        }
    }

    virtual ~Handler() = default;
};

class FatalErrorHandler : public Handler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::FATAL_ERROR) {
            throw runtime_error("[FATAL] " + msg.message());
        } else {
            Handler::handle(msg);  // Передаём следующему
        }
    }
};

class ErrorHandler : public Handler {
    string filePath_;
public:
    ErrorHandler(const string& path) : filePath_(path) {}

    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::ERROR) {
            ofstream file(filePath_, ios::app);
            if (file.is_open()) {
                file << "[ERROR] " << msg.message() << endl;
            }
        } else {
            Handler::handle(msg);
        }
    }
};

class WarningHandler : public Handler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::WARNING) {
            cout << "[WARNING] " << msg.message() << endl;
        } else {
            Handler::handle(msg);
        }
    }
};

class UnknownHandler : public Handler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::UNKNOWN) {
            throw runtime_error("Unhandled message: " + msg.message());
        } else {
            Handler::handle(msg);
        }
    }
};

int main() {
    // Создаём обработчики
    FatalErrorHandler fatalHandler;
    ErrorHandler errorHandler("errors.log");
    WarningHandler warningHandler;
    UnknownHandler unknownHandler;

    // Строим цепочку
    fatalHandler.setNext(&errorHandler);
    errorHandler.setNext(&warningHandler);
    warningHandler.setNext(&unknownHandler);

    // Тестовые сообщения
    LogMessage warning(Type::WARNING, "Disk space low");
    LogMessage error(Type::ERROR, "Invalid input");
    LogMessage fatal(Type::FATAL_ERROR, "Server crashed");
    LogMessage unknown(Type::UNKNOWN, "???");
    
    // Обрабатываем
    try {
        fatalHandler.handle(warning); 
        fatalHandler.handle(error);
        fatalHandler.handle(fatal);
        fatalHandler.handle(unknown);
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
    
    

    return 0;
}
