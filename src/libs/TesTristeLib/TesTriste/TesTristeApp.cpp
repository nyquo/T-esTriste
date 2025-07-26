#include "TesTristeApp.hpp"

#include <ctime>

namespace TesTriste {

TesTristeApp::TesTristeApp()
  : TesTriste::Application("TesTriste")
  , m_appContext(std::make_shared<AppContext>())
  , m_mainLayer(std::make_shared<MainLayer>(
      m_appContext, Size{ .width = getWindow().getWidth(), .height = getWindow().getHeight() })) {
    std::srand(std::time({}));
    getWindow().pushLayer(m_mainLayer);
}

void TesTristeApp::onEvent(TesTriste::Event& event) {
    TesTriste::Application::onEvent(event);

    TesTriste::EventDispatcher dispatcher(event);
    dispatcher.dispatch<TesTriste::WindowResizeEvent>(BIND_EVENT_FN(TesTristeApp::onWindowResized));
}

bool TesTristeApp::onWindowResized(TesTriste::WindowResizeEvent& event) {
    if(m_mainLayer) {
        m_mainLayer->setLayerSize(event.getSize());
    }
    return false;
}
}