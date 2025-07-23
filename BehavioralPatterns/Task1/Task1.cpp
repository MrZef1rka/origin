#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

class LogCommand
{
    public:
    virtual ~LogCommand() = default;
    virtual void print(const string& message) = 0;
};

class PrintInCons : public LogCommand
{
    public:
    void print(const string& message) override
    {
        cout << message << endl;
    }
};

class PrintInFile : public LogCommand
{
    string filePath;
    public:
    PrintInFile(const string& path) : filePath(path) {}
    
    void print(const string& message) override
    {
        ofstream outFile(filePath, ios::app);
        if (!outFile.is_open())
        {
            throw runtime_error("Can't open file: " + filePath);
        }
        
        outFile << message << endl;
        cout << "The message is recorded in " << filePath << endl;        
    }
};

void print(LogCommand& logger, const string& message)
{
    logger.print(message);
};

int main(int argc, char* argv[])
{
    PrintInCons consoleLogger;
    PrintInFile fileLogger("log.txt");

    print(consoleLogger, "Hello World!");
    print(fileLogger, "Hello World!");
    
    return 0;
}
