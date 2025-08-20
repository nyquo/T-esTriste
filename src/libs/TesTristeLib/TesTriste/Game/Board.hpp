#pragma once

#include <TesTristeLib/Events/Event.hpp>
#include <TesTristeLib/Events/KeyEvent.hpp>
#include <TesTristeLib/Renderer/PerspectiveCamera.hpp>
#include <TesTristeLib/TesTriste/AppContext.hpp>
#include <TesTristeLib/TesTriste/Game/Piece.hpp>
#include <TesTristeLib/TesTriste/Game/Pools.hpp>

#include <entt/entt.hpp>
#include <memory>
#include <testristelib_export.h>

namespace TesTriste {

struct TET_EXPORT CurrentFallingPieceComponent {
    glm::uvec3 presenceMatrixPos{ 0, 0, 0 };
};

struct TET_EXPORT ColorComponent {
    glm::vec3 color{ 0.0F, 0.0F, 0.0F };
};

class TET_EXPORT Board {
    using OptEntity = std::optional<entt::entity>;
    // PM -> Presence Matrix
    // 3D matrix that represent the presence or not of a piece in given space ([x][y][z])
    using PMLine = std::vector<OptEntity>; // line is on the z axis
    using PMSlice = std::vector<PMLine>;   // slice is on the y z plane
    using PresenceMatrix = std::vector<PMSlice>;
    struct TET_EXPORT GameLogicData {
        int currentFallingDelayMs{ 1000 };
        double lastFallingPieceTime{ 0.0 };
        bool currentFallingPieceReachedBottom{ true };
        bool gameStarted{ false };
        PresenceMatrix presenceMatrix{};
        int score{ 0 };
        bool gameLost{ false };
    };

  public:
    Board(std::shared_ptr<AppContext>,
          std::shared_ptr<PerspectiveCamera> camera,
          unsigned int boardWidth,
          unsigned int boardHeight,
          float cubeSize);
    Board(const Board& other) = delete;
    Board(Board&& other) = delete;
    Board& operator=(const Board& other) = delete;
    Board& operator=(Board&& other) = delete;

    void onEvent(Event& event);

    void onDraw();
    void onUpdate();
    void startGame();
    void pauseGame();
    int getScore();
    bool isGameLost();
    bool isGameStarted();

    void setFallingDelay(int delayMs) { m_gameLogicData.currentFallingDelayMs = delayMs; }

  private:
    bool onKeyPressed(KeyPressedEvent& e);

  private:
    void initRessources();
    void makePiecesFall();
    void addNewFallingPiece();
    void removeCompletedPlanes();
    void updatePiecesPos();
    std::vector<size_t> getPlanesToRemove() const;
    bool isPositionValid(const glm::uvec3& pos) const;
    bool isPositionOccupied(const glm::uvec3& pos) const;
    bool isPlaneEmpty(size_t y) const;
    bool isItTimeToMakePiecesFall() const;
    void moveFallingPieces(const glm::ivec3& translation);

  private:
    std::shared_ptr<AppContext> m_appContext;
    std::shared_ptr<PerspectiveCamera> m_camera;
    PiecePool m_piecePool;
    ColorPool m_colorPool;
    entt::registry m_registry;
    GameLogicData m_gameLogicData;

    const unsigned int BOARD_WIDTH;
    const unsigned int BOARD_HEIGHT;
    const float CUBE_SIZE;
};

}