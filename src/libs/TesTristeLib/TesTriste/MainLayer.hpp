#include <TesTristeLib/Core/Layer.hpp>
#include <TesTristeLib/Renderer/Buffers.hpp>
#include <TesTristeLib/Renderer/Shader.hpp>

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
    std::vector<float> m_triangleVertices{ -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    std::vector<unsigned int> m_triangleIndexs{ 0, 2, 1 };
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    VertexArray m_vertexArray;

    std::unique_ptr<Shader> m_shader;

    static constexpr char sep = std::filesystem::path::preferred_separator;
};

}