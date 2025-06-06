#pragma once

#include <TesTristeLib/Core/Window.hpp>
#include <TesTristeLib/Events/Event.hpp>
#include <TesTristeLib/Events/WindowEvent.hpp>

#include <atomic>
#include <memory>
#include <string>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT Application {
  public:
    Application(std::string mainWindowName = "HelloWord");
    Application(const Application& other) = delete;
    Application(Application&& other) = delete;
    Application operator=(const Application& other) = delete;
    Application operator=(Application&& other) = delete;
    virtual ~Application();

  public:
    virtual void onEvent(Event& event);
    void run();

  protected:
    [[nodiscard]] Window& getWindow() { return *m_mainWindow; }
    [[nodiscard]] const Window& getWindow() const { return *m_mainWindow; }

  private: // Event callbacks
    bool onWindowClose(WindowCloseEvent& event);

  private:
    std::unique_ptr<Window> m_mainWindow;
    std::atomic_bool m_continueRunning{ true };
};

}