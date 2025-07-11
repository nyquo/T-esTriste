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

class TET_EXPORT Board {
    // PM -> Presence Matrix
    // 3D matrix that represent the presence or not of a piece in given space ([x][y][z])
    using PMLine = std::vector<bool>;    // line is on the z axis
    using PMSlice = std::vector<PMLine>; // slice is on the y z plane
    using PresenceMatrix = std::vector<PMSlice>;
    struct TET_EXPORT GameLogicData {
        int currentFallingDelayMs{ 1000 };
        double lastFallingPieceTime{ 0.0 };
        bool currentFallingPieceReachedBottom{ false };
        std::unique_ptr<Piece> currentFallingPiece{ nullptr };
        PresenceMatrix presenceMatrix{};
    };

  public:
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

  private:
    bool onKeyPressed(KeyPressedEvent& e);

  private:
    void makePiecesFall();
    void populatePiecesPool();
    void populateColorPool();
    void addNewFallingPiece();

  private:
    std::vector<Piece> m_piecesPool;
    std::vector<glm::vec3> m_colorPool;
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