#include "MainLayer.hpp"

#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/Scene/Components.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace TesTriste {

MainLayer::MainLayer(float width, float height)
  : Layer(width, height)
  , m_camera(std::make_shared<PerspectiveCamera>(m_layerWidth, m_layerHeight, glm::vec3(0.0F, 0.0F, 10.0F))) {
    m_shader = std::make_unique<Shader>(
      std::string(RESSOURCES_FOLDER) + sep + "TesTriste" + sep + "Shaders" + sep + "BasicShader.vert",
      std::string(RESSOURCES_FOLDER) + sep + "TesTriste" + sep + "Shaders" + sep + "BasicShader.frag");

    auto entity = m_registry.create();
    m_registry.emplace<TesTriste::TransformComponent>(
      entity, glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(1.0F, 1.0F, 1.0F));
    for(auto x = -1; x <= 2; x += 2) {
        for(auto y = -1; y <= 2; y += 2) {
            for(auto z = -1; z <= 2; z += 2) {
                auto entity = m_registry.create();
                m_registry.emplace<TesTriste::TransformComponent>(
                  entity,
                  glm::vec3(x * s_cubeSize, y * s_cubeSize, z * s_cubeSize),
                  glm::vec3(0.0F, 0.0F, 0.0F),
                  glm::vec3(1.0F, 1.0F, 1.0F));
            }
        }
    }

    // OpenGL
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void MainLayer::onEvent(TesTriste::Event& event) { m_cameraMover.onEvent(event); }

void MainLayer::onUpdate() {
    glViewport(0, 0, m_layerWidth, m_layerHeight);
    // Todo Move this in an event when the window is resized
    m_camera->setViewPortSize(m_layerWidth, m_layerHeight);

    m_cameraMover.update();

    m_shader->bind();
    m_shader->setMat4("view", m_camera->getView());
    m_shader->setMat4("projection", m_camera->getProjection());
    m_shader->setVec3("meshColor", m_meshColor.r, m_meshColor.g, m_meshColor.b);
    m_shader->setFloat("meshSize", s_cubeSize);
    m_cube.bind();
    m_shader->setVec3("meshOrigin", glm::vec3(0.0F, 0.0F, 0.0F));

    auto view = m_registry.view<const TransformComponent>();
    for(const auto& entity : view) {
        const auto mat = view.get<const TransformComponent>(entity).getTransformMatrix();

        m_shader->setMat4("model", mat);
        glDrawElements(GL_TRIANGLES, m_cube.getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
}

void MainLayer::onImGuiRender() {
    showFps();

    ImGui::Begin("Settings");
    ImGui::DragFloat3("Mesh color", (float*)&m_meshColor, .01, 0.0f, 1.0f);
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