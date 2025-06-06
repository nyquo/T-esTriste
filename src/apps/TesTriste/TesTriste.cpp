#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/TesTriste/TesTristeApp.hpp>

#include <glm/glm.hpp>
#include <iostream>

int main() {
#ifndef NDEBUG
    TesTriste::Logger::setLogLevel(TesTriste::Logger::LogLevel::Debug);
#else
    TesTriste::Logger::setLogLevel(TesTriste::Logger::LogLevel::Debug);
#endif

    TesTriste::Logger::logInfo("Program started!");

    TesTriste::TesTristeApp app{};

    app.run();

    return 0;
}