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
#include <TesTristeLib/TesTriste/Shapes/Cube.hpp>

#include <entt/entt.hpp>
#include <filesystem>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT MainLayer : public TesTriste::Layer {
  public:
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
    entt::registry m_registry;
    std::shared_ptr<PerspectiveCamera> m_camera;
    CameraMover m_cameraMover{ m_camera };

    // TEMP
  private:
    MeshManager::MeshID m_cubeId;
    static constexpr int s_cubeSize = 1.0F;
    glm::vec3 m_meshColor{ 0.0f, 1.0f, 0.0f };
};

}