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
    ImGui::DragFloat3("Mesh color", (float*)&m_meshColor, .01, 0.0f, 1.0f);
    if(ImGui::Button("Play")) {
        m_board.startGame();
    }
    ImGui::SameLine();
    if(ImGui::Button("Pause")) {
        m_board.pauseGame();
    }
    ImGui::DragInt("Falling delay (ms)", &m_fallingDelayMs, 10.0f, 100, 5000);
    m_board.setFallingDelay(m_fallingDelayMs);
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

void MainLayer::drawScene() {
    // General environnement of the scene will be drawn here.
    // Thinks like the board and some decoration

    static const auto basicShaderId = m_appContext->shaderManager.hashShaderId("BasicShader");
    static const auto& basicShader = m_appContext->shaderManager.getShader(basicShaderId);

    static const auto boardGridId = m_appContext->meshManager.hashMeshId("BoardGrid");
    const auto boardMesh = dynamic_pointer_cast<BoardGrid>(m_appContext->meshManager.getMesh(boardGridId));

    basicShader->bind();
    basicShader->setMat4("view", m_camera->getView());
    basicShader->setMat4("projection", m_camera->getProjection());
    basicShader->setVec3("meshColor", m_meshColor);
    basicShader->setMat4("model",
                         glm::translate(glm::mat4(1.0F),
                                        glm::vec3(-static_cast<int>(s_boardWidth) / 2,
                                                  -boardMesh->getCellHeight(),
                                                  -static_cast<int>(s_boardWidth) / 2)));

    boardMesh->bind();
    glDrawElements(GL_TRIANGLES, boardMesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
}

bool MainLayer::onWindowResized(TesTriste::WindowResizeEvent& event) {
    glViewport(0, 0, static_cast<int>(event.getSize().width), static_cast<int>(event.getSize().height));
    m_camera->setViewPortSize(event.getSize());

    return false;
}

}