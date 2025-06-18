#include <TesTristeLib/Core/Layer.hpp>
#include <TesTristeLib/Renderer/Buffers.hpp>
#include <TesTristeLib/Renderer/PerspectiveCamera.hpp>
#include <TesTristeLib/Renderer/Shader.hpp>
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

    // TEMP
  private:
    Cube m_cube{ 10.0F };
    std::shared_ptr<PerspectiveCamera> m_camera;

    std::unique_ptr<Shader> m_shader;
    glm::vec3 m_meshColor{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_meshPosition{ 0.0f, 0.0f, 0.0f };
    glm::vec3 m_meshAngle{ 0.0f, 0.0f, 0.0f };

    entt::registry m_registry;

    static constexpr char sep = std::filesystem::path::preferred_separator;
};

}