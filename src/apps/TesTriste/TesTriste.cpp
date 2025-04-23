#include <TesTristeLib/Core/Logger.hpp>

#include <glm/glm.hpp>
#include <iostream>

int main() {
    glm::vec2 vec{ 0.0, 5.0 };
    TesTriste::Logger::setLogLevel(TesTriste::Logger::LogLevel::Info);
    TesTriste::Logger::logInfo("Program started! Here is a vec: ", vec.x, " , ", vec.y, "\n");
    return 0;
}