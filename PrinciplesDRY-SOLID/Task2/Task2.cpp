#include <fstream>
#include <memory>
#include <stdexcept>

// Интерфейс для форматирования данных (принцип разделения интерфейса)
class DataFormatter {
public:
    virtual ~DataFormatter() = default;
    virtual std::string format(const std::string& data) const = 0;
};

// Конкретные реализации форматирования (принцип открытости/закрытости)
class HTMLFormatter : public DataFormatter {
public:
    std::string format(const std::string& data) const override {
        return "<html>" + data + "</html>";
    }
};

class TextFormatter : public DataFormatter {
public:
    std::string format(const std::string& data) const override {
        return data;
    }
};

class JSONFormatter : public DataFormatter {
public:
    std::string format(const std::string& data) const override {
        return "{ \"data\": \"" + data + "\"}";
    }
};

// Класс для хранения данных (принцип единственной ответственности)
class Data {
public:
    explicit Data(std::string data) : data_(std::move(data)) {}

    const std::string& getRawData() const {
        return data_;
    }

private:
    std::string data_;
};

// Функции для сохранения в разных форматах
void saveTo(std::ofstream& file, const Data& data, const DataFormatter& formatter) {
    file << formatter.format(data.getRawData());
}

void saveToAsHTML(std::ofstream& file, const Data& data) {
    HTMLFormatter formatter;
    saveTo(file, data, formatter);
}

void saveToAsText(std::ofstream& file, const Data& data) {
    TextFormatter formatter;
    saveTo(file, data, formatter);
}

void saveToAsJSON(std::ofstream& file, const Data& data) {
    JSONFormatter formatter;
    saveTo(file, data, formatter);
    
}