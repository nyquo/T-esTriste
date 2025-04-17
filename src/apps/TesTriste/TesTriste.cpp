#include <glm/glm.hpp>
#include <imgui_impl_glfw.h>
#include <iostream>

int main() {
    glm::vec2 vec{ 0.0, 5.0 };
    std::cout << "Hello world!! Here is a vec: " << vec.x << "," << vec.y << "\n";
    return 0;
}