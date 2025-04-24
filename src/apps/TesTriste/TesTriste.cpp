#include <TesTristeLib/Core/Application.hpp>
#include <TesTristeLib/Core/Logger.hpp>

#include <glm/glm.hpp>
#include <iostream>

int main() {
#ifndef NDEBUG
    TesTriste::Logger::setLogLevel(TesTriste::Logger::LogLevel::Debug);
#else
    TesTriste::Logger::setLogLevel(TesTriste::Logger::LogLevel::Info);
#endif

    TesTriste::Logger::logInfo("Program started!");

    TesTriste::Application app;

    app.run();

    return 0;
}