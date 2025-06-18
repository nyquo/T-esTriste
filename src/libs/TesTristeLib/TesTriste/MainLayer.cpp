#include "MainLayer.hpp"

#include <TesTristeLib/Core/Logger.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace TesTriste {

MainLayer::MainLayer(float width, float height)
  : Layer(width, height)
  , m_camera(std::make_shared<PerspectiveCamera>(m_layerWidth, m_layerHeight, glm::vec3(0.0F, 10.0F, 50.0F))) {
    m_shader = std::make_unique<Shader>(
      std::string(RESSOURCES_FOLDER) + sep + "TesTriste" + sep + "Shaders" + sep + "BasicShader.vert",
      std::string(RESSOURCES_FOLDER) + sep + "TesTriste" + sep + "Shaders" + sep + "BasicShader.frag");
}

void MainLayer::onEvent(TesTriste::Event& event) {}

void MainLayer::onUpdate() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glViewport(0, 0, m_layerWidth, m_layerHeight);
    m_camera->setViewPortSize(m_layerWidth, m_layerHeight);
    m_shader->bind();
    m_shader->setMat4("view", m_camera->getView());
    m_shader->setMat4("projection", m_camera->getProjection());
    auto modelMatrix = glm::translate(glm::mat4(1.0F), m_meshPosition);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_meshAngle.x), glm::vec3(1.0F, 0.0F, 0.0F));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_meshAngle.y), glm::vec3(0.0F, 1.0F, 0.0F));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_meshAngle.z), glm::vec3(0.0F, 0.0F, 1.0F));
    m_shader->setMat4("model", modelMatrix);
    m_shader->setVec3("meshColor", m_meshColor.r, m_meshColor.g, m_meshColor.b);
    m_shader->setVec3("meshOrigin", glm::vec3(0.0F, 0.0F, 0.0F));
    m_shader->setFloat("meshSize", 10.0F);
    m_cube.bind();
    glDrawElements(GL_TRIANGLES, m_cube.getIndicesCount(), GL_UNSIGNED_INT, 0);
}

void MainLayer::onImGuiRender() {
    showFps();

    ImGui::Begin("Settings");
    ImGui::DragFloat3("Mesh color", (float*)&m_meshColor, .01, 0.0f, 1.0f);
    ImGui::DragFloat3("Mesh position", (float*)&m_meshPosition, .05, -50.0f, 50.0f);
    ImGui::DragFloat3("Mesh angle", (float*)&m_meshAngle, .1, -180.0f, 180.0f);
    ImGui::End();
}

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