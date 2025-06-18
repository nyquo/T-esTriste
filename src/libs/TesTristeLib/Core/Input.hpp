#pragma once

#include <TesTristeLib/Core/gl.hpp>
#include <TesTristeLib/TesTristeExport.hpp>

#include <glm/glm.hpp>

namespace core {

class TET_API Input {
  public:
    static bool isKeyPressed(int key) {
        auto* window = glfwGetCurrentContext();

        if(window == nullptr) {
            return false;
        }

        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    static bool isMouseButtonPressed(int button) {
        auto* window = glfwGetCurrentContext();

        if(window == nullptr) {
            return false;
        }

        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    static glm::vec2 getMousePosition() {
        auto* window = glfwGetCurrentContext();

        if(window == nullptr) {
            return { 0, 0 };
        }

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return { (float)x, (float)y };
    }
};

}
