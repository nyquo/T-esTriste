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

    // Initialize the presence matrix
    m_gameLogicData.presenceMatrix = PresenceMatrix(BOARD_WIDTH, PMSlice(BOARD_HEIGHT, PMLine(BOARD_WIDTH, false)));

    auto entity = m_registry.create();
    m_registry.emplace<TesTriste::TransformComponent>(
      entity, glm::vec3(0.0F, 0.0F + 15.5F, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(1.0F, 1.0F, 1.0F));
    m_registry.emplace<CurrentFallingPieceComponent>(entity);
    m_registry.emplace<ColorComponent>(entity, glm::vec3(1.0F, 0.0F, 0.0F));
    for(auto x = -1; x <= 2; x += 2) {
        for(auto y = -1; y <= 2; y += 2) {
            for(auto z = -1; z <= 2; z += 2) {
                auto entity = m_registry.create();
                m_registry.emplace<TesTriste::TransformComponent>(
                  entity,
                  glm::vec3(x * s_cubeSize, y * s_cubeSize + 15.5F, z * s_cubeSize),
                  glm::vec3(0.0F, 0.0F, 0.0F),
                  glm::vec3(1.0F, 1.0F, 1.0F));

                m_registry.emplace<CurrentFallingPieceComponent>(entity);
                m_registry.emplace<ColorComponent>(entity, glm::vec3(0.0F, 1.0F, 0.0F));
            }
        }
    }

    populatePiecesPool();
    populateColorPool();
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

void Board::onUpdate() {
    if(!m_gameStarted) {
        return;
    }
    makePiecesFall();
    if(m_gameLogicData.currentFallingPieceReachedBottom) {
        // generate a new piece
    }
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

    // clang-format on
}

void Board::populateColorPool() {
    m_colorPool.emplace_back(1.0F, 0.0F, 0.0F); // Red
    m_colorPool.emplace_back(0.0F, 1.0F, 0.0F); // Green
    m_colorPool.emplace_back(0.0F, 0.0F, 1.0F); // Blue
    m_colorPool.emplace_back(1.0F, 1.0F, 0.0F); // Yellow
}

void Board::addNewFallingPiece() {
    if(m_piecesPool.empty() || m_colorPool.empty()) {
        return;
    }

    size_t randomIndex = rand() % m_piecesPool.size();
    const auto& piece = m_piecesPool[randomIndex];

    // for(const auto& slice : piece) {
    //     for(const auto& row : slice) {
    //         for(const auto& cell : row) {
    //             if(cell) {
    //                 // auto x =
    //                 // if(m_gameLogicData.presenceMatrix[cell]) {
    //                 //     // TODO if the cell is already occupied, we cannot add this piece, call game over func
    //                 //     return;
    //                 // }
    //                 auto entity = m_registry.create();
    //                 m_registry.emplace<TesTriste::TransformComponent>(entity,
    //                                                                   glm::vec3(0.0F, 0.0F + 15.5F, 0.0F),
    //                                                                   glm::vec3(0.0F, 0.0F, 0.0F),
    //                                                                   glm::vec3(1.0F, 1.0F, 1.0F));
    //                 m_registry.emplace<CurrentFallingPieceComponent>(entity);
    //                 m_registry.emplace<ColorComponent>(entity, glm::vec3(1.0F, 0.0F, 0.0F));
    //             }
    //         }
    //     }
    // }

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
        transformCmp.translation += translation;
    }
    return false;
}
}