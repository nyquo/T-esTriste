#include "MainLayer.hpp"

#include <TesTristeLib/Core/Logger.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace TesTriste {

void MainLayer::onEvent(TesTriste::Event& event) {}
void MainLayer::onUpdate() {}
void MainLayer::onImGuiRender() {
    const float fps = ImGui::GetIO().Framerate;
    ImGui::Begin("FPS");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::End();
}

}