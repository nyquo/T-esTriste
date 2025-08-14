#pragma once

#include <TesTristeLib/Core/Layer.hpp>
#include <TesTristeLib/Events/WindowEvent.hpp>
#include <TesTristeLib/Renderer/Buffers.hpp>
#include <TesTristeLib/Renderer/PerspectiveCamera.hpp>
#include <TesTristeLib/Renderer/Shader.hpp>
#include <TesTristeLib/Scene/CameraMover.hpp>
#include <TesTristeLib/TesTriste/AppContext.hpp>
#include <TesTristeLib/TesTriste/Game/Board.hpp>
#include <TesTristeLib/TesTriste/Shapes/BoardGrid.hpp>
#include <TesTristeLib/TesTriste/Shapes/Corner.hpp>

#include <filesystem>
#include <memory>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT MainLayer : public TesTriste::Layer {
  public:
    static constexpr unsigned int s_boardWidth{ 7 };
    static constexpr unsigned int s_boardHeight{ 7 };
    static constexpr float s_cubeSize{ 1 };

    MainLayer(std::shared_ptr<AppContext> appContext, Size size = Size{ .width = 800, .height = 600 });
    MainLayer(const MainLayer& other) = delete;
    MainLayer(MainLayer&& other) = delete;
    MainLayer operator=(const MainLayer& other) = delete;
    MainLayer operator=(MainLayer&& other) = delete;
    ~MainLayer() override = default;

    void onEvent(TesTriste::Event& event) override;
    void onUpdate() override;
    void onImGuiRender() override;

  private:
    void initRessources();
    void showFps();
    void drawScene();

    // Event handlers
  private:
    bool onWindowResized(TesTriste::WindowResizeEvent& event);

  private:
    std::shared_ptr<PerspectiveCamera> m_camera;
    CameraMover m_cameraMover{ m_camera };
    std::shared_ptr<AppContext> m_appContext;
    Board m_board{ m_appContext, m_camera, s_boardWidth, s_boardHeight, s_cubeSize };

    // Temp
    int m_fallingDelayMs{ 1000 };
};

}