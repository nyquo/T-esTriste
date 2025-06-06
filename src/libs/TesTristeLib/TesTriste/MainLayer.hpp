#include <TesTristeLib/Core/Layer.hpp>

#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT MainLayer : public TesTriste::Layer {
  public:
    MainLayer() = default;
    MainLayer(const MainLayer& other) = delete;
    MainLayer(MainLayer&& other) = delete;
    MainLayer operator=(const MainLayer& other) = delete;
    MainLayer operator=(MainLayer&& other) = delete;
    ~MainLayer() override = default;

    void onEvent(TesTriste::Event& event) override;
    void onUpdate() override;
    void onImGuiRender() override;
};

}