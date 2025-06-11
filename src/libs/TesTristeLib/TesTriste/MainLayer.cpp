#include "MainLayer.hpp"

#include <TesTristeLib/Core/Logger.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace TesTriste {

MainLayer::MainLayer()
  : m_vertexBuffer(m_triangleVertices.size() * sizeof(float), m_triangleVertices.data())
  , m_indexBuffer(m_triangleIndexs.size() * sizeof(unsigned int), m_triangleIndexs.data()) {
    BufferLayout layout{ BufferElement(GL_FLOAT, 3, GL_FALSE, sizeof(float)) };
    m_vertexBuffer.setLayout(std::move(layout));
    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuffer);

    glBindVertexArray(0);

    glDisable(GL_CULL_FACE);

    m_shader = std::make_unique<Shader>(
      std::string(RESSOURCES_FOLDER) + sep + "TesTriste" + sep + "shaders" + sep + "BasicShader.vert",
      std::string(RESSOURCES_FOLDER) + sep + "TesTriste" + sep + "shaders" + sep + "BasicShader.frag");
}

void MainLayer::onEvent(TesTriste::Event& event) {}

void MainLayer::onUpdate() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_shader->bind();
    m_vertexArray.bind();
    glDrawElements(GL_TRIANGLES, m_triangleIndexs.size(), GL_UNSIGNED_INT, 0);
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