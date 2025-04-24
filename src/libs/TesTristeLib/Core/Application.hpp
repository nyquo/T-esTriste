#pragma once

#include <TesTristeLib/Core/Window.hpp>
#include <TesTristeLib/Events/Event.hpp>
#include <TesTristeLib/Events/WindowEvent.hpp>
#include <TesTristeLib/TesTristeExport.hpp>

#include <atomic>
#include <memory>
#include <string>

namespace TesTriste {

class TET_API Application {
  public:
    Application(std::string mainWindowName = "HelloWord");
    Application(const Application& other) = delete;
    Application(Application&& other) = delete;
    Application operator=(const Application& other) = delete;
    Application operator=(Application&& other) = delete;
    virtual ~Application();

  public:
    virtual void onEvent(Event& e);
    void run();

  private: // Event callbacks
    bool onWindowClose(WindowCloseEvent& e);

  private:
    std::unique_ptr<Window> m_mainWindow;
    std::atomic_bool m_continueRunning{ true };
};

}