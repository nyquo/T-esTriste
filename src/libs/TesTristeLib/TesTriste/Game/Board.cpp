#include "Board.hpp"

#include <TesTristeLib/Scene/Components.hpp>
#include <TesTristeLib/TesTriste/Shapes/Cube.hpp>

namespace TesTriste {
Board::Board(const std::shared_ptr<PerspectiveCamera> camera, unsigned int boardWidth, unsigned int boardHeight)
  : m_camera(camera)
  , BOARD_WIDTH(boardWidth)
  , BOARD_HEIGHT(boardHeight) {
    const char sep = std::filesystem::path::preferred_separator;
    const std::string ressourceFolder = std::string(RESSOURCES_FOLDER);
    const std::string shaderFolder = ressourceFolder + sep + "TesTriste" + sep + "Shaders" + sep;

    // Load all shader
    // TODO maybe this should be done elsewhere?
    m_shaderManager.addShader(shaderFolder + "CubeShader.vert", shaderFolder + "CubeShader.frag", "BasicCubeShader");

    // Load all meshes
    // TODO maybe this should be also done elsewhere?
    m_cubeId = m_meshManager.addMesh(std::make_unique<Cube>(s_cubeSize));

    auto entity = m_registry.create();
    m_registry.emplace<TesTriste::TransformComponent>(
      entity, glm::vec3(0.0F, 0.0F + 4, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(1.0F, 1.0F, 1.0F));
    m_registry.emplace<CurrentFallingPieceComponent>(entity);
    m_registry.emplace<ColorComponent>(entity, glm::vec3(1.0F, 0.0F, 0.0F));
    for(auto x = -1; x <= 2; x += 2) {
        for(auto y = -1; y <= 2; y += 2) {
            for(auto z = -1; z <= 2; z += 2) {
                auto entity = m_registry.create();
                m_registry.emplace<TesTriste::TransformComponent>(
                  entity,
                  glm::vec3(x * s_cubeSize, y * s_cubeSize + 4, z * s_cubeSize),
                  glm::vec3(0.0F, 0.0F, 0.0F),
                  glm::vec3(1.0F, 1.0F, 1.0F));

                m_registry.emplace<CurrentFallingPieceComponent>(entity);
                m_registry.emplace<ColorComponent>(entity, glm::vec3(0.0F, 1.0F, 0.0F));
            }
        }
    }
}

void Board::onDraw() {
    static const auto cubeShaderId = m_shaderManager.hashShaderId("BasicCubeShader");
    auto& cubeShader = m_shaderManager.getShader(cubeShaderId);

    cubeShader.bind();

    cubeShader.setMat4("view", m_camera->getView());
    cubeShader.setMat4("projection", m_camera->getProjection());
    cubeShader.setFloat("meshSize", s_cubeSize);
    cubeShader.setVec3("meshOrigin", glm::vec3(0.0F, 0.0F, 0.0F));
    m_meshManager.getMesh(m_cubeId).bind();

    auto view = m_registry.view<const TransformComponent, ColorComponent>();
    for(auto [entity, transformCmp, colorCmp] : view.each()) {
        cubeShader.setVec3("meshColor", colorCmp.color);
        const auto mat = transformCmp.getTransformMatrix();

        cubeShader.setMat4("model", mat);
        glDrawElements(GL_TRIANGLES, m_meshManager.getMesh(m_cubeId).getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
}

void Board::onUpdate() {}

void Board::onEvent(TesTriste::Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Board::onKeyPressed));
}

bool Board::onKeyPressed(KeyPressedEvent& e) {
    glm::vec3 translation{ 0.0F, 0.0F, 0.0F };
    if(e.getKeyCode() == GLFW_KEY_A) {
        translation = { -1.0F, 0.0F, 0.0F };
    } else if(e.getKeyCode() == GLFW_KEY_D) {
        translation = { 1.0F, 0.0F, 0.0F };
    } else if(e.getKeyCode() == GLFW_KEY_W) {
        translation = { 0.0F, 0.0F, -1.0F };
    } else if(e.getKeyCode() == GLFW_KEY_S) {
        translation = { 0.0F, 0.0F, 1.0F };
    }
    auto view = m_registry.view<TransformComponent, CurrentFallingPieceComponent>();
    for(auto entity : view) {
        auto& transformCmp = view.get<TransformComponent>(entity);
        transformCmp.translation += translation;
    }
    return false;
}
}