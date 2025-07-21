#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept> 

enum class LogType 
{
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage 
{
private:
    LogType _type;
    std::string _message;
public:
    LogMessage(LogType type, const std::string& message) : _type(type), _message(message) {}

    LogType type() const { return _type; }
    const std::string& message() const { return _message; }
};

class MessageHandler 
{
protected:
    std::unique_ptr<MessageHandler> nextHandler; 

public:
    virtual ~MessageHandler() = default;

    
    MessageHandler* setNext(std::unique_ptr<MessageHandler> handler) 
    {
        nextHandler = std::move(handler);
        return nextHandler.get();
    }

    virtual void handle(const LogMessage& logMsg) = 0;
};

class FatalErrorHandler : public MessageHandler 
{
public:
    void handle(const LogMessage& logMsg) override 
    {
        if (logMsg.type() == LogType::FatalError) 
        {
            throw std::runtime_error("FATAL ERROR: " + logMsg.message());
        }
        else if (nextHandler) 
        { 
            nextHandler->handle(logMsg); 
        }
        
    }
};

class ErrorHandler : public MessageHandler 
{
private:
    std::string filePath;
public:
    ErrorHandler(const std::string& path) : filePath(path) {}

    void handle(const LogMessage& logMsg) override 
    {
        if (logMsg.type() == LogType::Error) 
        {
            std::ofstream outFile(filePath, std::ios::app);
            if (outFile.is_open()) 
            {
                outFile << "ERROR LOG to file: " << logMsg.message() << std::endl;
                outFile.close();
            }
            else 
            {
                std::cerr << "Error: Unable to open error handler file " << filePath << std::endl;
            }
        }
        else if (nextHandler) 
        {
            nextHandler->handle(logMsg);
        }
    }
};

class WarningHandler : public MessageHandler 
{
public:
    void handle(const LogMessage& logMsg) override 
    {
        if (logMsg.type() == LogType::Warning) 
        {
            std::cout << "WARNING to console: " << logMsg.message() << std::endl;
        }
        else if (nextHandler) 
        {
            nextHandler->handle(logMsg);
        }
    }
};

class UnknownMessageHandler : public MessageHandler 
{
public:
    void handle(const LogMessage& logMsg) override 
    {
        if (logMsg.type() == LogType::Unknown) 
        {
            throw std::runtime_error("UNKNOWN MESSAGE: Unhandled log type for message: " + logMsg.message());
        }
        else 
        {
            
            throw std::runtime_error("CRITICAL: Message of type " + std::to_string(static_cast<int>(logMsg.type())) +
                " passed through chain without being handled: " + logMsg.message());
        }
        
    }
};


void task3_demo() 
{
    auto fatalHandler = std::make_unique<FatalErrorHandler>();
    
    fatalHandler->setNext(std::make_unique<ErrorHandler>("chain_error_log.txt"))
        ->setNext(std::make_unique<WarningHandler>())
        ->setNext(std::make_unique<UnknownMessageHandler>());

    try 
    {
        LogMessage warnMsg(LogType::Warning, "Это сообщение-предупреждение в цепочке.");
        fatalHandler->handle(warnMsg);

        LogMessage errMsg(LogType::Error, "Это сообщение-ошибка в цепочке.");
        fatalHandler->handle(errMsg);

        LogMessage unknownMsg(LogType::Unknown, "Это неизвестное сообщение в цепочке.");
        fatalHandler->handle(unknownMsg); 
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Перехвачено исключение: " << e.what() << std::endl;
    }

    try 
    {
        LogMessage fatalMsg(LogType::FatalError, "Это фатальная ошибка в цепочке!");
        fatalHandler->handle(fatalMsg); 
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Перехвачено исключение (фатальная): " << e.what() << std::endl;
    }

    
    try 
    {
        LogMessage unhandledMsg(static_cast<LogType>(999), "Это сообщение с неизвестным типом (не Unknown)."); 
        fatalHandler->handle(unhandledMsg); 
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Перехвачено исключение (необработанное): " << e.what() << std::endl;
    }

    std::cout << std::endl;
}

