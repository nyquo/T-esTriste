#pragma once

#include <TesTristeLib/Core/gl.hpp>

#include <glm/glm.hpp>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT Input {
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
