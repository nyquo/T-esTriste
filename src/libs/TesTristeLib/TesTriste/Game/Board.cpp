#include "Board.hpp"

#include <TesTristeLib/Scene/Components.hpp>
#include <TesTristeLib/TesTriste/Shapes/Cube.hpp>

#include <ctime>

namespace TesTriste {
Board::Board(const std::shared_ptr<PerspectiveCamera> camera, unsigned int boardWidth, unsigned int boardHeight)
  : m_camera(camera)
  , BOARD_WIDTH(boardWidth)
  , BOARD_HEIGHT(boardHeight) {
    std::srand(std::time({}));

    const char sep = std::filesystem::path::preferred_separator;
    const std::string ressourceFolder = std::string(RESSOURCES_FOLDER);
    const std::string shaderFolder = ressourceFolder + sep + "TesTriste" + sep + "Shaders" + sep;

    // Load all shader
    // TODO maybe this should be done elsewhere?
    m_shaderManager.addShader(shaderFolder + "CubeShader.vert", shaderFolder + "CubeShader.frag", "BasicCubeShader");

    // Load all meshes
    // TODO maybe this should be also done elsewhere?
    m_cubeId = m_meshManager.addMesh(std::make_unique<Cube>(s_cubeSize));

    // Initialize the presence matrix
    m_gameLogicData.presenceMatrix = PresenceMatrix(BOARD_WIDTH, PMSlice(BOARD_HEIGHT, PMLine(BOARD_WIDTH, false)));

    populatePiecesPool();
    populateColorPool();
    addNewFallingPiece();
}

void Board::onDraw() {
    static const auto cubeShaderId = m_shaderManager.hashShaderId("BasicCubeShader");
    auto& cubeShader = m_shaderManager.getShader(cubeShaderId);

    cubeShader.bind();

    cubeShader.setMat4("view", m_camera->getView());
    cubeShader.setMat4("projection", m_camera->getProjection());
    cubeShader.setFloat("meshSize", s_cubeSize);
    cubeShader.setVec3("meshOrigin", glm::vec3(0.5F, 0.5F, 0.5F));
    m_meshManager.getMesh(m_cubeId).bind();

    auto view = m_registry.view<const TransformComponent, ColorComponent>();
    for(auto [entity, transformCmp, colorCmp] : view.each()) {
        cubeShader.setVec3("meshColor", colorCmp.color);
        const auto mat = transformCmp.getTransformMatrix();

        cubeShader.setMat4("model", mat);
        glDrawElements(GL_TRIANGLES, m_meshManager.getMesh(m_cubeId).getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
}

void Board::onUpdate() {
    if(!m_gameStarted) {
        return;
    }
    if(m_gameLogicData.currentFallingPieceReachedBottom) {
        for(auto entity : m_registry.view<CurrentFallingPieceComponent>()) {
            m_registry.remove<CurrentFallingPieceComponent>(entity);
        }
        m_gameLogicData.currentFallingPieceReachedBottom = false;
        m_gameLogicData.currentFallingPiece = nullptr;
        addNewFallingPiece();
    }
    makePiecesFall();
}

void Board::onEvent(TesTriste::Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Board::onKeyPressed));
}

void Board::startGame() {
    m_gameStarted = true;
    m_gameLogicData.lastFallingPieceTime = glfwGetTime();
}

void Board::pauseGame() { m_gameStarted = false; }

void Board::makePiecesFall() {
    double currentTime = glfwGetTime();
    double currentFallingDelayS = m_gameLogicData.currentFallingDelayMs / 1000.0F;
    double timeSinceLastFalling = currentTime - m_gameLogicData.lastFallingPieceTime;
    if(timeSinceLastFalling >= currentFallingDelayS) {
        m_gameLogicData.lastFallingPieceTime = currentTime - (timeSinceLastFalling - currentFallingDelayS);

        auto view = m_registry.view<TransformComponent, CurrentFallingPieceComponent>();

        // Check if the current falling piece can fall
        for(auto entity : view) {
            auto& transformCmp = view.get<TransformComponent>(entity);
            if(transformCmp.translation.y <= 0.0F) {
                m_gameLogicData.currentFallingPieceReachedBottom = true;
                return; // The piece reached the bottom, stop falling
            }
        }

        for(auto entity : view) {
            auto& transformCmp = view.get<TransformComponent>(entity);
            transformCmp.translation.y -= s_cubeSize;
        }
    }
}

void Board::populatePiecesPool() {
    // clang-format off
    m_piecesPool.emplace_back(std::vector<glm::ivec3>{
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 2, 0, 0 },
        { 3, 0, 0 }
    });

    m_piecesPool.emplace_back(std::vector<glm::ivec3>{
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 2, 0, 0 },
        { 3, 0, 0 },
        { 0, 1, 0 }
    });

    // clang-format on
}

void Board::populateColorPool() {
    m_colorPool.emplace_back(1.0F, 0.0F, 0.0F);                                  // Red
    m_colorPool.emplace_back(0.0F, 1.0F, 0.0F);                                  // Green
    m_colorPool.emplace_back(0.0F, 0.0F, 1.0F);                                  // Blue
    m_colorPool.emplace_back(1.0F, 1.0F, 0.0F);                                  // Yellow
    m_colorPool.emplace_back(230.0F / 255.0F, 108.0F / 255.0F, 191.0F / 255.0F); // Purple-ish
    m_colorPool.emplace_back(1.0F, 1.0F, 1.0F);                                  // White
}

void Board::addNewFallingPiece() {
    if(m_piecesPool.empty() || m_colorPool.empty()) {
        return;
    }

    size_t randomPieceIndex = rand() % m_piecesPool.size();
    const auto& piece = m_piecesPool[randomPieceIndex];

    size_t randomColorIndex = rand() % m_colorPool.size();
    const auto& color = m_colorPool[randomColorIndex];

    for(const auto& pos : piece.getCubePositions()) {
        glm::vec3 position = glm::vec3((pos.x - static_cast<int>(piece.getWidth()) / 2) * s_cubeSize,
                                       (pos.y + BOARD_HEIGHT) * s_cubeSize,
                                       (pos.z - static_cast<int>(piece.getDepth()) / 2) * s_cubeSize);
        auto entity = m_registry.create();
        m_registry.emplace<TesTriste::TransformComponent>(entity, position, glm::vec3(0.0F), glm::vec3(1.0F));
        m_registry.emplace<ColorComponent>(entity, color);
        m_registry.emplace<CurrentFallingPieceComponent>(entity);
    }

    m_gameLogicData.currentFallingPiece = std::make_unique<Piece>(piece);
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
        if(transformCmp.translation.x + translation.x < static_cast<int>(-BOARD_WIDTH) / 2 ||
           transformCmp.translation.x + translation.x > static_cast<int>(BOARD_WIDTH) / 2 - 1 ||
           transformCmp.translation.z + translation.z < static_cast<int>(-BOARD_WIDTH) / 2 ||
           transformCmp.translation.z + translation.z > static_cast<int>(BOARD_WIDTH) / 2 - 1) {
            return false; // Prevent moving out of bounds
        }
    }
    for(auto entity : view) {
        auto& transformCmp = view.get<TransformComponent>(entity);
        transformCmp.translation += translation;
    }
    return false;
}
}