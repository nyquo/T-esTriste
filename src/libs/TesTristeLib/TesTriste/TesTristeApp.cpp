#include "TesTristeApp.hpp"

namespace TesTriste {

TesTristeApp::TesTristeApp()
  : TesTriste::Application("TesTriste")
  , m_mainLayer(std::make_shared<MainLayer>()) {
    getWindow().pushLayer(m_mainLayer);
}

void TesTristeApp::onEvent(TesTriste::Event& event) {
    TesTriste::Application::onEvent(event);

    TesTriste::EventDispatcher dispatcher(event);
    dispatcher.dispatch<TesTriste::WindowResizeEvent>(BIND_EVENT_FN(TesTristeApp::onWindowResized));
}

bool TesTristeApp::onWindowResized(TesTriste::WindowResizeEvent& event) {
    if(m_mainLayer) {
        m_mainLayer->setLayerSize(event.getWidth(), event.getHeight());
    }
    return false;
}

}