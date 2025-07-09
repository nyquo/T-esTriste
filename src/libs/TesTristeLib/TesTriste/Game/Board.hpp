#pragma once

#include <TesTristeLib/Events/Event.hpp>
#include <TesTristeLib/Events/KeyEvent.hpp>
#include <TesTristeLib/Renderer/MeshManager.hpp>
#include <TesTristeLib/Renderer/PerspectiveCamera.hpp>
#include <TesTristeLib/Renderer/ShaderManager.hpp>
#include <TesTristeLib/TesTriste/Game/Piece.hpp>

#include <entt/entt.hpp>
#include <memory>
#include <testristelib_export.h>

namespace TesTriste {

struct TET_EXPORT CurrentFallingPieceComponent {};

struct TET_EXPORT ColorComponent {
    glm::vec3 color{ 0.0F, 0.0F, 0.0F };
};

// For now the drawing, logic, etc is all done in the same class, maybe this should be separated in different "systems"
// later

struct GameLogicData {
    int currentFallingDelayMs{ 1000 };
    double lastFallingPieceTime{ 0.0 };
    bool currentFallingPieceReachedBottom{ false };
};

class TET_EXPORT Board {
  public:
    // 3D matrix that represent the presence of piece on the board
    // array[width][height][width] -> array[x][y][z]
    // using BoardPresenceMatrix = std::array<std::array<std::array<bool, s_width>, s_height>, s_width>;

    Board(const std::shared_ptr<PerspectiveCamera> camera, unsigned int boardWidth = 15, unsigned int boardHeight = 40);
    Board(const Board& other) = delete;
    Board(Board&& other) = delete;
    Board& operator=(const Board& other) = delete;
    Board& operator=(Board&& other) = delete;

    void onEvent(Event& event);

    void onDraw();
    void onUpdate();
    void startGame();
    void pauseGame();
    void makePiecesFall();

  private:
    bool onKeyPressed(KeyPressedEvent& e);

  private:
    // BoardPresenceMatrix m_presenceMatrix;
    std::shared_ptr<PerspectiveCamera> m_camera;

    ShaderManager m_shaderManager;
    MeshManager m_meshManager;
    entt::registry m_registry;

    MeshManager::MeshID m_cubeId;
    static constexpr int s_cubeSize = 1.0F;
    const unsigned int BOARD_WIDTH;
    const unsigned int BOARD_HEIGHT;

    bool m_gameStarted{ false };

    GameLogicData m_gameLogicData;
};

}