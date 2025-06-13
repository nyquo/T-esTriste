#include <TesTristeLib/Core/Layer.hpp>
#include <TesTristeLib/Renderer/Buffers.hpp>
#include <TesTristeLib/Renderer/Shader.hpp>
#include <TesTristeLib/TesTriste/Shapes/Cube.hpp>

#include <entt/entt.hpp>
#include <filesystem>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT MainLayer : public TesTriste::Layer {
  public:
    MainLayer();
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
    Cube m_cube{ 1.0F };

    std::unique_ptr<Shader> m_shader;

    entt::registry m_registry;

    static constexpr char sep = std::filesystem::path::preferred_separator;
};

}