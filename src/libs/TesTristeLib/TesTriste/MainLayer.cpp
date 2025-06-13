#include "MainLayer.hpp"

#include <TesTristeLib/Core/Logger.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace TesTriste {

MainLayer::MainLayer() {
    glDisable(GL_CULL_FACE);

    m_shader = std::make_unique<Shader>(
      std::string(RESSOURCES_FOLDER) + sep + "TesTriste" + sep + "Shaders" + sep + "BasicShader.vert",
      std::string(RESSOURCES_FOLDER) + sep + "TesTriste" + sep + "Shaders" + sep + "BasicShader.frag");
}

void MainLayer::onEvent(TesTriste::Event& event) {}

void MainLayer::onUpdate() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_shader->bind();
    m_cube.bind();
    glDrawElements(GL_TRIANGLES, m_cube.getIndicesCount(), GL_UNSIGNED_INT, 0);
}

void MainLayer::onImGuiRender() { showFps(); }

void MainLayer::showFps() {
    const float fps = ImGui::GetIO().Framerate;
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoFocusOnAppearing;
    ImGui::Begin("FPS", nullptr, windowFlags);
    ImGui::Text("FPS: %.1f", fps);
    ImGui::End();
    ImGui::PopStyleVar();
}
}