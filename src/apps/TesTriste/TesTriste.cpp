#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/Io/ProgramLocation.hpp>
#include <TesTristeLib/TesTriste/TesTristeApp.hpp>

#include <glm/glm.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
#ifndef NDEBUG
    TesTriste::Logger::setLogLevel(TesTriste::Logger::LogLevel::Debug);
#else
    TesTriste::Logger::setLogLevel(TesTriste::Logger::LogLevel::Debug);
#endif

    TesTriste::Logger::logInfo("Program started!");
    TesTriste::ProgramLocation::setProgramLocation(std::filesystem::path(argv[0]).parent_path());

    TesTriste::TesTristeApp app{};

    app.run();

    return 0;
}