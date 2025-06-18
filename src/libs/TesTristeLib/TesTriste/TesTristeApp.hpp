#include "MainLayer.hpp"

#include <TesTristeLib/Core/Application.hpp>
#include <TesTristeLib/Events/WindowEvent.hpp>

#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT TesTristeApp : public TesTriste::Application {
  public:
    TesTristeApp();

    void onEvent(TesTriste::Event& event) override;

  private:
    bool onWindowResized(TesTriste::WindowResizeEvent& event);

  private:
    std::shared_ptr<MainLayer> m_mainLayer;
};

}