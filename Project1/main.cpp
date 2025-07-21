#include <iostream>
#include <fstream>
#include <string>
#include <memory> 

class LogCommand 
{
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand 
{
public:
    void print(const std::string& message) override 
    {
        std::cout << "Console Log: " << message << std::endl;
    }
};

class FileLogCommand : public LogCommand 
{
private:
    std::string filePath;
public:
    FileLogCommand(const std::string& path) : filePath(path) {}

    void print(const std::string& message) override 
    {
        std::ofstream outFile(filePath, std::ios::app); 
        if (outFile.is_open()) {
            outFile << "File Log: " << message << std::endl;
            outFile.close();
        }
        else 
        {
            std::cerr << "Error: Unable to open file " << filePath << std::endl;
        }
    }
};

void executeLogCommand(LogCommand& command, const std::string& message) 
{
    command.print(message);
}

void task1_demo() 
{
    std::cout << "--- ������� 1: ������� ������� ---" << std::endl;

    ConsoleLogCommand consoleCmd;
    FileLogCommand fileCmd("log.txt");

    executeLogCommand(consoleCmd, "��� ��������� � �������.");
    executeLogCommand(fileCmd, "��� ��������� � ����.");

    std::cout << std::endl;
}