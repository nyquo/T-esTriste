#include "Board.hpp"

#include <TesTristeLib/Io/ProgramLocation.hpp>
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
    const std::string ressourceFolder =
      ProgramLocation::getProgramLocation().string() + sep + std::string(RESSOURCES_FOLDER);
    const std::string shaderFolder = ressourceFolder + sep + "TesTriste" + sep + "Shaders" + sep;

    // Load all shader
    // TODO maybe this should be done elsewhere?
    m_shaderManager.addShader(shaderFolder + "CubeShader.vert", shaderFolder + "CubeShader.frag", "BasicCubeShader");

    // Load all meshes
    // TODO maybe this should be also done elsewhere?
    m_cubeId = m_meshManager.addMesh(std::make_unique<Cube>(s_cubeSize));

    // Initialize the presence matrix
    m_gameLogicData.presenceMatrix =
      PresenceMatrix(BOARD_WIDTH, PMSlice(BOARD_HEIGHT, PMLine(BOARD_WIDTH, std::nullopt)));

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
        addNewFallingPiece();
    }
    makePiecesFall();
    removeCompletedPlanes();
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
            auto& currentFallingPieceCmp = view.get<CurrentFallingPieceComponent>(entity);
            if(currentFallingPieceCmp.presenceMatrixPos.y <= 0.0F) {
                m_gameLogicData.currentFallingPieceReachedBottom = true;
                break; // The piece reached the bottom, stop falling
            }
            auto decreasedPos = currentFallingPieceCmp.presenceMatrixPos - glm::uvec3(0.0F, 1.0F, 0.0F);
            if(isPositionOccupied(decreasedPos)) {
                m_gameLogicData.currentFallingPieceReachedBottom = true;
                break; // The piece reached another piece, stop falling
            }
        }

        if(m_gameLogicData.currentFallingPieceReachedBottom) {
            for(auto entity : view) {
                auto& currentFallingPieceCmp = view.get<CurrentFallingPieceComponent>(entity);
                auto pos = currentFallingPieceCmp.presenceMatrixPos;
                if(isPositionValid(pos)) {
                    m_gameLogicData.presenceMatrix[pos.x][pos.y][pos.z] = entity;
                }
            }
            return;
        }

        for(auto entity : view) {
            auto& transformCmp = view.get<TransformComponent>(entity);
            transformCmp.translation.y -= s_cubeSize;
            auto& currentFallingPieceCmp = view.get<CurrentFallingPieceComponent>(entity);
            currentFallingPieceCmp.presenceMatrixPos.y -= 1.0F;
        }
    }
}

void Board::populatePiecesPool() {
    // clang-format off
    m_piecesPool.emplace_back(std::vector<glm::uvec3>{
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 2, 0, 0 },
        { 3, 0, 0 },
        { 4, 0, 0 }
    });

    /*m_piecesPool.emplace_back(std::vector<glm::uvec3>{
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 2, 0, 0 },
        { 3, 0, 0 },
        { 0, 1, 0 }
    });

    m_piecesPool.emplace_back(std::vector<glm::uvec3>{
        { 1, 0, 1 },
        { 1, 1, 1 },
        { 1, 1, 0 },
        { 1, 1, 2 },
        { 0, 1, 1 },
        { 2, 1, 1 }
    });

    m_piecesPool.emplace_back(std::vector<glm::uvec3>{
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 2, 0, 0 },
        { 0, 0, 1 },
        { 1, 0, 1 },
        { 2, 0, 1 }
    });*/
    // clang-format on
}

void Board::populateColorPool() {
    m_colorPool.emplace_back(1.0F, 0.0F, 0.0F);                       // Red
    m_colorPool.emplace_back(0.0F, 1.0F, 0.0F);                       // Green
    m_colorPool.emplace_back(0.0F, 0.0F, 1.0F);                       // Blue
    m_colorPool.emplace_back(1.0F, 1.0F, 0.0F);                       // Yellow
    m_colorPool.emplace_back(0.901960784F, 0.423529412, 0.749019608); // Purple-ish
    m_colorPool.emplace_back(1.0F, 1.0F, 1.0F);                       // White
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
        glm::vec3 position = glm::vec3((static_cast<int>(pos.x) - static_cast<int>(piece.getWidth()) / 2) * s_cubeSize,
                                       (static_cast<int>(pos.y) + BOARD_HEIGHT) * s_cubeSize,
                                       (static_cast<int>(pos.z) - static_cast<int>(piece.getDepth()) / 2) * s_cubeSize);
        glm::vec3 presenceMatrixPos = glm::uvec3(BOARD_WIDTH / 2 - static_cast<int>(piece.getWidth()) / 2 + pos.x,
                                                 pos.y + BOARD_HEIGHT,
                                                 BOARD_WIDTH / 2 - static_cast<int>(piece.getDepth()) / 2 + pos.z);
        auto entity = m_registry.create();
        m_registry.emplace<TesTriste::TransformComponent>(entity, position, glm::vec3(0.0F), glm::vec3(1.0F));
        m_registry.emplace<ColorComponent>(entity, color);
        m_registry.emplace<CurrentFallingPieceComponent>(entity, presenceMatrixPos);
    }
}

void Board::removeCompletedPlanes() {
    auto planesToRemove = getPlanesToRemove();

    if(planesToRemove.empty()) {
        return;
    }

    for(auto y : planesToRemove) {
        for(size_t x = 0; x < BOARD_WIDTH; ++x) {
            for(size_t z = 0; z < BOARD_WIDTH; ++z) {
                if(m_registry.valid(m_gameLogicData.presenceMatrix[x][y][z].value())) {
                    m_registry.destroy(m_gameLogicData.presenceMatrix[x][y][z].value());
                }
                m_gameLogicData.presenceMatrix[x][y][z] = std::nullopt;
            }
        }
    }
    size_t yToShift = planesToRemove[0] + 1;
    size_t emptyPlanesHandled = 1;
    while(true) {
        if(yToShift >= BOARD_HEIGHT) {
            break; // No more planes to shift
        }
        if(isPlaneEmpty(yToShift) && emptyPlanesHandled >= planesToRemove.size()) {
            break;
        }
        // move this plane down;
        if(std::find(planesToRemove.begin(), planesToRemove.end(), yToShift) != planesToRemove.end()) {
            emptyPlanesHandled++;
        }
        yToShift++;
    }
}

std::vector<size_t> Board::getPlanesToRemove() const {
    std::vector<size_t> planesToRemove;
    for(size_t y = 0; y < BOARD_HEIGHT; ++y) {
        bool fullPlane = true;
        for(size_t x = 0; x < BOARD_WIDTH; ++x) {
            for(size_t z = 0; z < BOARD_WIDTH; ++z) {
                if(!m_gameLogicData.presenceMatrix[x][y][z]) {
                    fullPlane = false;
                    break;
                }
            }
            if(!fullPlane) {
                break;
            }
        }
        if(fullPlane) {
            planesToRemove.push_back(y);
        }
    }
    return std::move(planesToRemove);
}

bool Board::isPositionValid(const glm::uvec3& pos) const {
    if(pos.x < 0 || pos.y < 0 || pos.z < 0) {
        return false; // Negative indices are invalid
    }
    if(pos.x >= m_gameLogicData.presenceMatrix.size()) {
        return false;
    }
    if(pos.y >= m_gameLogicData.presenceMatrix[pos.x].size()) {
        return false;
    }
    if(pos.z >= m_gameLogicData.presenceMatrix[pos.x][pos.y].size()) {
        return false;
    }
    return true;
}

bool Board::isPositionOccupied(const glm::uvec3& pos) const {
    if(isPositionValid(pos)) {
        return m_gameLogicData.presenceMatrix[pos.x][pos.y][pos.z].has_value();
    }
    return false; // Position is out of bounds
}

bool Board::isPlaneEmpty(size_t y) const {
    if(y >= m_gameLogicData.presenceMatrix[0].size()) {
        return false;
    }
    for(size_t x = 0; x < m_gameLogicData.presenceMatrix.size(); ++x) {
        for(size_t z = 0; z < m_gameLogicData.presenceMatrix[x][y].size(); ++z) {
            if(m_gameLogicData.presenceMatrix[x][y][z].has_value()) {
                return false;
            }
        }
    }
    return true;
}

bool Board::onKeyPressed(KeyPressedEvent& e) {
    glm::ivec3 translation{ 0, 0, 0 };
    if(e.getKeyCode() == GLFW_KEY_A) {
        translation = { -1, 0, 0 };
    } else if(e.getKeyCode() == GLFW_KEY_D) {
        translation = { 1, 0, 0 };
    } else if(e.getKeyCode() == GLFW_KEY_W) {
        translation = { 0, 0, -1 };
    } else if(e.getKeyCode() == GLFW_KEY_S) {
        translation = { 0, 0, 1 };
    }
    auto view = m_registry.view<TransformComponent, CurrentFallingPieceComponent>();
    for(auto entity : view) {
        auto& currentFallingPieceCmp = view.get<CurrentFallingPieceComponent>(entity);
        glm::ivec3 translatedPos = glm::ivec3(currentFallingPieceCmp.presenceMatrixPos) + translation;
        if(translatedPos.x < 0 || translatedPos.z < 0 || translatedPos.x >= BOARD_WIDTH ||
           translatedPos.z >= BOARD_WIDTH) {
            return false; // Prevent moving out of bounds
        }
    }
    for(auto entity : view) {
        auto& transformCmp = view.get<TransformComponent>(entity);
        transformCmp.translation += translation;
        auto& currentFallingPieceCmp = view.get<CurrentFallingPieceComponent>(entity);
        currentFallingPieceCmp.presenceMatrixPos += translation;
    }
    return false;
}
}