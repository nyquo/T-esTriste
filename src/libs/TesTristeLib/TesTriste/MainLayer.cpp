#include "MainLayer.hpp"

#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/Events/Event.hpp>
#include <TesTristeLib/Io/ProgramLocation.hpp>
#include <TesTristeLib/Scene/Components.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace TesTriste {

MainLayer::MainLayer(std::shared_ptr<AppContext> appContext, Size size)
  : Layer(size)
  , m_appContext(std::move(appContext))
  , m_camera(std::make_shared<PerspectiveCamera>(m_layerSize, glm::vec3(0.0F, 10.0F, 10.0F))) {
    // Init camera pos
    m_cameraMover.moveCamera(0.0F, 0.0F);

    initRessources();
}

void MainLayer::onEvent(TesTriste::Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<TesTriste::WindowResizeEvent>(BIND_EVENT_FN(MainLayer::onWindowResized));
    m_cameraMover.onEvent(event);
    m_board.onEvent(event);
}

void MainLayer::onUpdate() {
    m_cameraMover.update();
    m_board.onUpdate();
    m_board.onDraw();
    drawScene();
}

void MainLayer::onImGuiRender() {
    showFps();

    ImGui::Begin("Settings");

    if(m_board.isGameStarted()) {
        if(ImGui::Button("Pause")) {
            m_board.pauseGame();
        }
        ImGui::Text("Current Score %d", m_board.getScore());
    } else if(!m_board.isGameLost()) {
        if(ImGui::Button("Play")) {
            m_board.startGame();
        }
    } else {
        ImGui::Text("Game ended. Final Score: %d", m_board.getScore());
    }
    ImGui::End();
}

void MainLayer::initRessources() {
    const char sep = std::filesystem::path::preferred_separator;
    const std::string ressourceFolder =
      ProgramLocation::getProgramLocation().string() + sep + std::string(RESSOURCES_FOLDER);
    const std::string shaderFolder = ressourceFolder + sep + "TesTriste" + sep + "Shaders" + sep;
    m_appContext->shaderManager.addShader(
      shaderFolder + "BasicShader.vert", shaderFolder + "BasicShader.frag", "BasicShader");
    m_appContext->meshManager.addMesh(std::make_unique<BoardGrid>(s_cubeSize, s_boardWidth), "BoardGrid");
    m_appContext->meshManager.addMesh(std::make_unique<Corner>(0.2F), "Corner");
}

void MainLayer::showFps() {
    const ImVec2 windowPos{ 10, 10 };
    const float windowTransparency{ 0.5f };

    const float fps = ImGui::GetIO().Framerate;

    ImGui::SetNextWindowPos(windowPos);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::SetNextWindowBgAlpha(windowTransparency);
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoFocusOnAppearing;
    ImGui::Begin("FPS", nullptr, windowFlags);
    ImGui::Text("FPS: %.1f", fps);
    ImGui::End();
    ImGui::PopStyleVar();
}

void MainLayer::drawScene() {
    // General environnement of the scene will be drawn here.
    // Thinks like the board and some decoration

    static const auto basicShaderId = ShaderManager::hashShaderId("BasicShader");
    static const auto& basicShader = m_appContext->shaderManager.getShader(basicShaderId);

    basicShader->bind();
    basicShader->setMat4("view", m_camera->getView());
    basicShader->setMat4("projection", m_camera->getProjection());

    static const auto boardGridId = MeshManager::hashMeshId("BoardGrid");
    const auto boardMesh = dynamic_pointer_cast<BoardGrid>(m_appContext->meshManager.getMesh(boardGridId));
    const auto boardColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    basicShader->setVec4("meshColor", boardColor);
    basicShader->setMat4("model",
                         glm::translate(glm::mat4(1.0F),
                                        glm::vec3(-static_cast<float>(s_boardWidth) / 2,
                                                  -boardMesh->getCellHeight(),
                                                  -static_cast<float>(s_boardWidth) / 2)));
    boardMesh->bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(boardMesh->getIndicesCount()), GL_UNSIGNED_INT, nullptr);

    static const auto cornerId = MeshManager::hashMeshId("Corner");
    const auto cornerMesh = dynamic_pointer_cast<Corner>(m_appContext->meshManager.getMesh(cornerId));
    const auto cornerColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.8f);
    basicShader->setVec4("meshColor", cornerColor);
    cornerMesh->bind();

    glm::vec3 bottomLeftPos = glm::vec3(-static_cast<float>(s_boardWidth) / 2 - cornerMesh->getSize().x / 2,
                                        s_boardHeight - cornerMesh->getSize().y / 2,
                                        static_cast<float>(s_boardWidth) / 2 + cornerMesh->getSize().z / 2);
    auto bottomLeftModel = glm::translate(glm::mat4(1.0F), bottomLeftPos);

    glm::vec3 bottomRightPos = glm::vec3(static_cast<float>(s_boardWidth) / 2 + cornerMesh->getSize().x / 2,
                                         s_boardHeight - cornerMesh->getSize().y / 2,
                                         static_cast<float>(s_boardWidth) / 2 + cornerMesh->getSize().z / 2);
    auto bottomRightModel =
      glm::rotate(glm::translate(glm::mat4(1.0F), bottomRightPos), glm::pi<float>() / 2, glm::vec3(0.0F, 1.0F, 0.0F));

    glm::vec3 topLeftPos = glm::vec3(-static_cast<float>(s_boardWidth) / 2 - cornerMesh->getSize().x / 2,
                                     s_boardHeight - cornerMesh->getSize().y / 2,
                                     -static_cast<float>(s_boardWidth) / 2 - cornerMesh->getSize().z / 2);
    auto topLeftModel =
      glm::rotate(glm::translate(glm::mat4(1.0F), topLeftPos), -glm::pi<float>() / 2, glm::vec3(0.0F, 1.0F, 0.0F));

    glm::vec3 topRightPos = glm::vec3(static_cast<float>(s_boardWidth) / 2 + cornerMesh->getSize().x / 2,
                                      s_boardHeight - cornerMesh->getSize().y / 2,
                                      -static_cast<float>(s_boardWidth) / 2 - cornerMesh->getSize().z / 2);
    auto topRightModel =
      glm::rotate(glm::translate(glm::mat4(1.0F), topRightPos), glm::pi<float>(), glm::vec3(0.0F, 1.0F, 0.0F));

    basicShader->setMat4("model", bottomLeftModel);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cornerMesh->getIndicesCount()), GL_UNSIGNED_INT, nullptr);
    basicShader->setMat4("model", bottomRightModel);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cornerMesh->getIndicesCount()), GL_UNSIGNED_INT, nullptr);
    basicShader->setMat4("model", topLeftModel);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cornerMesh->getIndicesCount()), GL_UNSIGNED_INT, nullptr);
    basicShader->setMat4("model", topRightModel);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cornerMesh->getIndicesCount()), GL_UNSIGNED_INT, nullptr);
}

bool MainLayer::onWindowResized(TesTriste::WindowResizeEvent& event) {
    glViewport(0, 0, static_cast<int>(event.getSize().width), static_cast<int>(event.getSize().height));
    m_camera->setViewPortSize(event.getSize());

    return false;
}

}