#pragma once

#include <TesTristeLib/TesTristeExport.hpp>
#include <ctime>
#include <iostream>

namespace TesTriste {

class TET_API Logger {
  public:
    enum class LogLevel { Error, Warning, Info, Debug, Trace };

    static void setLogLevel(LogLevel logLevel) { s_logLevel = logLevel; }

    template<typename... Ts>
    static void logError(const Ts&... error) {
        log(LogLevel::Error, error...);
    }

    template<typename... Ts>
    static void logWarning(const Ts&... warning) {
        log(LogLevel::Warning, warning...);
    }

    template<typename... Ts>
    static void logInfo(const Ts&... info) {
        log(LogLevel::Info, info...);
    }

    template<typename... Ts>
    static void logDebug(const Ts&... debug) {
        log(LogLevel::Debug, debug...);
    }

    template<typename... Ts>
    static void logTrace(const Ts&... trace) {
        log(LogLevel::Trace, trace...);
    }

  private:
    template<typename... Ts>
    static void log(LogLevel level, const Ts&... t) {
        if(s_logLevel < level) {
            return;
        }
        std::time_t time = std::time(nullptr);
        char strTime[50];
        std::strftime(strTime, sizeof(strTime), "%H:%M:%S", std::localtime(&time));
        std::cout << logLevelToStr(level) << " ";
        (std::cout << ... << t) << std::endl;
    }

    static std::string logLevelToStr(LogLevel level) {
        switch(level) {
            case LogLevel::Error: return "[ERROR]"; break;
            case LogLevel::Warning: return "[WARNING]"; break;
            case LogLevel::Info: return "[INFO]"; break;
            case LogLevel::Debug: return "[DEBUG]"; break;
            case LogLevel::Trace: return "[TRACE]"; break;
            default: return "[UNEXISTING_LOG_LEVEL]"; break;
        }
    }

    static LogLevel s_logLevel;
};

}