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
}

}