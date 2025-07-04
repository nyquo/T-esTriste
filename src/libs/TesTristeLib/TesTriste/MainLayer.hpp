#pragma once

#include <TesTristeLib/Core/Layer.hpp>
#include <TesTristeLib/Events/WindowEvent.hpp>
#include <TesTristeLib/Renderer/Buffers.hpp>
#include <TesTristeLib/Renderer/MeshManager.hpp>
#include <TesTristeLib/Renderer/PerspectiveCamera.hpp>
#include <TesTristeLib/Renderer/Shader.hpp>
#include <TesTristeLib/Renderer/ShaderManager.hpp>
#include <TesTristeLib/Scene/CameraMover.hpp>
#include <TesTristeLib/TesTriste/Game/Board.hpp>
#include <TesTristeLib/TesTriste/Shapes/BoardGrid.hpp>

#include <filesystem>
#include <testristelib_export.h>

// TODO: May be worth a try to create a basic Renderer with a Command queue
namespace TesTriste {

class TET_EXPORT MainLayer : public TesTriste::Layer {
  public:
    static constexpr unsigned int s_boardWidth{ 15 };
    static constexpr unsigned int s_boardHeight{ 15 };

    MainLayer(float width = 800.0F, float height = 800.0F);
    MainLayer(const MainLayer& other) = delete;
    MainLayer(MainLayer&& other) = delete;
    MainLayer operator=(const MainLayer& other) = delete;
    MainLayer operator=(MainLayer&& other) = delete;
    ~MainLayer() override = default;

    void onEvent(TesTriste::Event& event) override;
    void onUpdate() override;
    void onImGuiRender() override;

  private:
    void showFps();
    void drawScene();

    // Event handlers
  private:
    bool onWindowResized(TesTriste::WindowResizeEvent& event);

  private:
    ShaderManager m_shaderManager;
    MeshManager m_meshManager;
    std::shared_ptr<PerspectiveCamera> m_camera;
    CameraMover m_cameraMover{ m_camera };
    Board m_board{ m_camera, s_boardWidth, s_boardHeight };

    glm::vec3 m_meshColor{ 0.2f, 0.2f, 0.2f };

    MeshManager::MeshID m_boardGridId{};
};

}