#include "Board.hpp"

#include <TesTristeLib/Io/ProgramLocation.hpp>
#include <TesTristeLib/Scene/Components.hpp>
#include <TesTristeLib/TesTriste/Shapes/Cube.hpp>

#include <array>

namespace TesTriste {
Board::Board(std::shared_ptr<AppContext> appContext,
             std::shared_ptr<PerspectiveCamera> camera,
             unsigned int boardWidth,
             unsigned int boardHeight,
             float cubeSize)
  : m_appContext(std::move(appContext))
  , m_camera(std::move(camera))
  , BOARD_WIDTH(boardWidth)
  , BOARD_HEIGHT(boardHeight)
  , CUBE_SIZE(cubeSize) {
    m_gameLogicData.presenceMatrix =
      PresenceMatrix(BOARD_WIDTH, PMSlice(BOARD_HEIGHT, PMLine(BOARD_WIDTH, std::nullopt)));
    initRessources();
}

void Board::onDraw() {
    static const auto cubeShaderId = m_appContext->shaderManager.hashShaderId("BasicCubeShader");
    static const auto& cubeShader = m_appContext->shaderManager.getShader(cubeShaderId);

    cubeShader->bind();

    cubeShader->setMat4("view", m_camera->getView());
    cubeShader->setMat4("projection", m_camera->getProjection());
    cubeShader->setFloat("meshSize", CUBE_SIZE);
    float middle = CUBE_SIZE / 2;
    cubeShader->setVec3("meshOrigin", glm::vec3(middle, middle, middle));

    static const auto cubeId = m_appContext->meshManager.hashMeshId("coloredCubeMesh");
    const auto cube = m_appContext->meshManager.getMesh(cubeId);

    cube->bind();

    auto view = m_registry.view<const TransformComponent, ColorComponent>();

    for(auto [entity, transformCmp, colorCmp] : view.each()) {
        cubeShader->setVec3("meshColor", colorCmp.color);
        const auto mat = transformCmp.getTransformMatrix();

        cubeShader->setMat4("model", mat);
        glDrawElements(GL_TRIANGLES, cube->getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
}

void Board::onUpdate() {
    if(!m_gameLogicData.gameStarted) {
        return;
    }
    if(m_gameLogicData.currentFallingPieceReachedBottom) {
        for(auto entity : m_registry.view<CurrentFallingPieceComponent>()) {
            m_registry.remove<CurrentFallingPieceComponent>(entity);
        }
        m_gameLogicData.currentFallingPieceReachedBottom = false;
        addNewFallingPiece();
    }
    if(isItTimeToMakePiecesFall()) {
        double currentTime = glfwGetTime();
        double timeSinceLastFalling = currentTime - m_gameLogicData.lastFallingPieceTime;
        double currentFallingDelayS = m_gameLogicData.currentFallingDelayMs / 1000.0F;
        m_gameLogicData.lastFallingPieceTime =
          currentTime - (timeSinceLastFalling - currentFallingDelayS); // Compensate in case of low fps

        makePiecesFall();
    }

    removeCompletedPlanes();
    updatePiecesPos();
}

void Board::onEvent(TesTriste::Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Board::onKeyPressed));
}

void Board::startGame() {
    m_gameLogicData.gameStarted = true;
    m_gameLogicData.lastFallingPieceTime = glfwGetTime();
}

void Board::pauseGame() { m_gameLogicData.gameStarted = false; }

void Board::initRessources() {
    const char sep = std::filesystem::path::preferred_separator;
    const std::string ressourceFolder =
      ProgramLocation::getProgramLocation().string() + sep + std::string(RESSOURCES_FOLDER);
    const std::string shaderFolder = ressourceFolder + sep + "TesTriste" + sep + "Shaders" + sep;

    m_appContext->shaderManager.addShader(
      shaderFolder + "CubeShader.vert", shaderFolder + "CubeShader.frag", "BasicCubeShader");

    m_appContext->meshManager.addMesh(std::make_shared<Cube>(CUBE_SIZE), "coloredCubeMesh");
}

void Board::makePiecesFall() {
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
                m_gameLogicData.presenceMatrix.at(pos.x).at(pos.y).at(pos.z) = entity;
            }
        }
        return;
    }

    for(auto entity : view) {
        auto& transformCmp = view.get<TransformComponent>(entity);
        transformCmp.translation.y -= CUBE_SIZE;
        auto& currentFallingPieceCmp = view.get<CurrentFallingPieceComponent>(entity);
        currentFallingPieceCmp.presenceMatrixPos.y -= 1.0F;
    }
}

void Board::addNewFallingPiece() {
    const auto& piece = m_piecePool.getRandomPiece();
    const auto& color = m_colorPool.getRandomColor();

    for(const auto& pos : piece.getCubePositions()) {
        glm::vec3 position =
          glm::vec3((static_cast<float>(pos.x) - static_cast<float>(piece.getWidth()) / 2) * CUBE_SIZE,
                    (static_cast<float>(pos.y) + BOARD_HEIGHT) * CUBE_SIZE,
                    (static_cast<float>(pos.z) - static_cast<float>(piece.getDepth()) / 2) * CUBE_SIZE);
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
                if(m_registry.valid(m_gameLogicData.presenceMatrix.at(x).at(y).at(z).value())) {
                    m_registry.destroy(m_gameLogicData.presenceMatrix[x][y][z].value());
                }
                m_gameLogicData.presenceMatrix.at(x).at(y).at(z) = std::nullopt;
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
        if(std::find(planesToRemove.begin(), planesToRemove.end(), yToShift) != planesToRemove.end()) {
            emptyPlanesHandled++;
        } else {
            // move this plane down;
            for(size_t x = 0; x < BOARD_WIDTH; ++x) {
                for(size_t z = 0; z < BOARD_WIDTH; ++z) {
                    m_gameLogicData.presenceMatrix.at(x).at(yToShift - emptyPlanesHandled).at(z) =
                      m_gameLogicData.presenceMatrix.at(x).at(yToShift).at(z);
                    m_gameLogicData.presenceMatrix.at(x).at(yToShift).at(z) = std::nullopt;
                }
            }
        }
        yToShift++;
    }
}

void Board::updatePiecesPos() {
    for(size_t x = 0; x < BOARD_WIDTH; ++x) {
        for(size_t y = 0; y < BOARD_HEIGHT; ++y) {
            for(size_t z = 0; z < BOARD_WIDTH; ++z) {
                if(m_gameLogicData.presenceMatrix.at(x).at(y).at(z).has_value()) {
                    auto entity = m_gameLogicData.presenceMatrix[x][y][z].value();
                    auto& transformCmp = m_registry.get<TransformComponent>(entity);
                    transformCmp.translation =
                      glm::vec3((static_cast<int>(x) - static_cast<float>(BOARD_WIDTH) / 2) * CUBE_SIZE,
                                static_cast<int>(y) * CUBE_SIZE,
                                (static_cast<int>(z) - static_cast<float>(BOARD_WIDTH) / 2) * CUBE_SIZE);
                }
            }
        }
    }
}

std::vector<size_t> Board::getPlanesToRemove() const {
    std::vector<size_t> planesToRemove;
    for(size_t y = 0; y < BOARD_HEIGHT; ++y) {
        bool fullPlane = true;
        for(size_t x = 0; x < BOARD_WIDTH; ++x) {
            for(size_t z = 0; z < BOARD_WIDTH; ++z) {
                if(!m_gameLogicData.presenceMatrix.at(x).at(y).at(z)) {
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
    return planesToRemove;
}

bool Board::isPositionValid(const glm::uvec3& pos) const {
    if(pos.x < 0 || pos.y < 0 || pos.z < 0) {
        return false; // Negative indices are invalid
    }
    if(pos.x >= m_gameLogicData.presenceMatrix.size()) {
        return false;
    }
    if(pos.y >= m_gameLogicData.presenceMatrix.at(pos.x).size()) {
        return false;
    }
    if(pos.z >= m_gameLogicData.presenceMatrix.at(pos.x).at(pos.y).size()) {
        return false;
    }
    return true;
}

bool Board::isPositionOccupied(const glm::uvec3& pos) const {
    if(isPositionValid(pos)) {
        return m_gameLogicData.presenceMatrix.at(pos.x).at(pos.y).at(pos.z).has_value();
    }
    return false; // Position is out of bounds
}

bool Board::isPlaneEmpty(size_t y) const {
    if(y >= m_gameLogicData.presenceMatrix.at(0).size()) {
        return false;
    }
    for(size_t x = 0; x < m_gameLogicData.presenceMatrix.size(); ++x) {
        for(size_t z = 0; z < m_gameLogicData.presenceMatrix.at(x).at(y).size(); ++z) {
            if(m_gameLogicData.presenceMatrix.at(x).at(y).at(z).has_value()) {
                return false;
            }
        }
    }
    return true;
}

bool Board::isItTimeToMakePiecesFall() const {
    double currentTime = glfwGetTime();
    double currentFallingDelayS = m_gameLogicData.currentFallingDelayMs / 1000.0F;
    double timeSinceLastFalling = currentTime - m_gameLogicData.lastFallingPieceTime;
    return timeSinceLastFalling >= currentFallingDelayS;
}

void Board::moveFallingPieces(const glm::ivec3& translation) {
    auto view = m_registry.view<TransformComponent, CurrentFallingPieceComponent>();
    for(auto entity : view) {
        auto& currentFallingPieceCmp = view.get<CurrentFallingPieceComponent>(entity);
        glm::ivec3 translatedPos = glm::ivec3(currentFallingPieceCmp.presenceMatrixPos) + translation;
        if(translatedPos.x < 0 || translatedPos.z < 0 || translatedPos.x >= BOARD_WIDTH ||
           translatedPos.z >= BOARD_WIDTH) {
            return; // Prevent moving out of bounds
        }
    }
    for(auto entity : view) {
        auto& transformCmp = view.get<TransformComponent>(entity);
        transformCmp.translation += translation;
        auto& currentFallingPieceCmp = view.get<CurrentFallingPieceComponent>(entity);
        currentFallingPieceCmp.presenceMatrixPos += translation;
    }
}

bool Board::onKeyPressed(KeyPressedEvent& e) {
    if(!m_gameLogicData.gameStarted) {
        return false;
    }
    if(e.getKeyCode() == GLFW_KEY_SPACE) {
        while(!m_gameLogicData.currentFallingPieceReachedBottom) {
            makePiecesFall();
        }
        return false;
    }

    // Find the correct direction to move pieces given the camera direction
    auto cameraDirection = glm::normalize(m_camera->getDirection());
    // front, back, left, right
    std::array<std::pair<float, glm::vec3>, 4> dots = {
        { { glm::dot(glm::vec3(0.0F, 0.0f, -1.0f), cameraDirection), glm::vec3(0.0F, 0.0F, -1.0F) },
          { glm::dot(glm::vec3(0.0F, 0.0f, 1.0f), cameraDirection), glm::vec3(0.0F, 0.0F, 1.0F) },
          { glm::dot(glm::vec3(-1.0F, 0.0f, 0.0f), cameraDirection), glm::vec3(-1.0F, 0.0F, 0.0F) },
          { glm::dot(glm::vec3(1.0F, 0.0f, 0.0f), cameraDirection), glm::vec3(1.0F, 0.0F, 0.0F) } }
    };

    auto forward =
      std::max_element(dots.begin(), dots.end(), [](auto& l, auto& r) { return l.first < r.first; })->second;
    auto right = glm::cross(forward, glm::vec3(0.0F, 1.0F, 0.0F));

    glm::ivec3 translation{ 0, 0, 0 };
    if(e.getKeyCode() == GLFW_KEY_A) {
        translation -= right;
    } else if(e.getKeyCode() == GLFW_KEY_D) {
        translation += right;
    } else if(e.getKeyCode() == GLFW_KEY_W) {
        translation += forward;
    } else if(e.getKeyCode() == GLFW_KEY_S) {
        translation -= forward;
    }
    moveFallingPieces(translation);

    return false;
}
}