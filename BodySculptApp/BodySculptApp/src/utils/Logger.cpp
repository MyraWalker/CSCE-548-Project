#include "Logger.h"
#include <fstream>
#include <ctime>

static std::string getTimestamp()
{
    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local);
    return std::string(buffer);
}

void Logger::Write(const std::string& level, const std::string& message)
{
    std::ofstream log("app.log", std::ios::app);

    log << getTimestamp()
        << " [" << level << "] "
        << message
        << std::endl;
}

void Logger::Info(const std::string& message)
{
    Write("INFO", message);
}

void Logger::Warn(const std::string& message)
{
    Write("WARN", message);
}

void Logger::Error(const std::string& message)
{
    Write("ERROR", message);
}