#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm> 

class Observer 
{
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {};
    virtual void onError(const std::string& message) {};
    virtual void onFatalError(const std::string& message) {};
};

class LoggerSubject 
{
private:
    std::vector<Observer*> observers;

    void removeObserver(Observer* obsToRemove) 
    {
        observers.erase(std::remove(observers.begin(), observers.end(), obsToRemove), observers.end());
    }

public:
    void addObserver(Observer* observer) 
    {
        if (std::find(observers.begin(), observers.end(), observer) == observers.end()) 
        {
            observers.push_back(observer);
        }
    }

    void removeObserver(Observer& observer) 
    {
        removeObserver(&observer);
    }

    void warning(const std::string& message) const 
    {
        for (Observer* obs : observers) 
        {
            if (obs) 
            { 
                obs->onWarning(message);
            }
        }
    }

    void error(const std::string& message) const 
    {
        for (Observer* obs : observers) 
        {
            if (obs) 
            {
                obs->onError(message);
            }
        }
    }

    void fatalError(const std::string& message) const 
    {
        for (Observer* obs : observers) 
        {
            if (obs) 
            {
                obs->onFatalError(message);
            }
        }
    }
};


class WarningConsoleObserver : public Observer 
{
public:
    void onWarning(const std::string& message) override 
    {
        std::cout << "ÎÁÚÅÊÒ ÍÀÁËÞÄÅÍÈß (WARN): " << message << std::endl;
    }
};

class ErrorFileObserver : public Observer 
{
private:
    std::string filePath;
public:
    ErrorFileObserver(const std::string& path) : filePath(path) {}

    void onError(const std::string& message) override 
    {
        std::ofstream outFile(filePath, std::ios::app);
        if (outFile.is_open()) 
        {
            outFile << "ÎÁÚÅÊÒ ÍÀÁËÞÄÅÍÈß (ERROR): " << message << std::endl;
            outFile.close();
        }
        else 
        {
            std::cerr << "Error: Unable to open error log file " << filePath << std::endl;
        }
    }
};

class FatalErrorObserver : public Observer 
{
private:
    std::string filePath;
public:
    FatalErrorObserver(const std::string& path) : filePath(path) {}

    void onFatalError(const std::string& message) override 
    {
        std::cout << "ÎÁÚÅÊÒ ÍÀÁËÞÄÅÍÈß (FATAL): " << message << std::endl;

        std::ofstream outFile(filePath, std::ios::app);
        if (outFile.is_open()) {
            outFile << "ÎÁÚÅÊÒ ÍÀÁËÞÄÅÍÈß (FATAL): " << message << std::endl;
            outFile.close();
        }
        else {
            std::cerr << "Error: Unable to open fatal error log file " << filePath << std::endl;
        }
    }
};

void task2_demo() 
{
    LoggerSubject logger;

    WarningConsoleObserver warnObserver;
    ErrorFileObserver errorObserver("error_log.txt");
    FatalErrorObserver fatalObserver("fatal_error_log.txt");

    logger.addObserver(&warnObserver);
    logger.addObserver(&errorObserver);
    logger.addObserver(&fatalObserver);

    logger.warning("Ýòî ïðåäóïðåæäåíèå!");
    logger.error("Ýòî îáû÷íàÿ îøèáêà.");
    logger.fatalError("Ýòî ôàòàëüíàÿ îøèáêà!");

    logger.removeObserver(warnObserver);
    logger.error("Ýòî îøèáêà ïîñëå óäàëåíèÿ íàáëþäàòåëÿ ïðåäóïðåæäåíèé. Warning íå áóäåò âûâåäåí.");

    

    std::cout << std::endl;
}