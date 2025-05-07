#include "TesTristeLib/Core/LayerStack.hpp"

#include "TesTristeLib/Core/Window.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>

namespace TesTriste {

LayerStack::LayerStack(Window* window)
  : m_window(window) {
    if(window == nullptr) {
        throw std::invalid_argument("Layer stack created without a window");
    }
}

void LayerStack::pushLayer(const std::shared_ptr<Layer>& layer) {
    if(std::find(m_layers.begin(), m_layers.end(), layer) == m_layers.end()) {
        m_layers.push_back(layer);
    }
}

void LayerStack::pushOverlayLayer(const std::shared_ptr<Layer>& layer) {
    if(std::find(m_uiLayers.begin(), m_uiLayers.end(), layer) == m_uiLayers.end()) {
        m_uiLayers.push_back(layer);
    }
}

void LayerStack::removeLayer(const std::shared_ptr<Layer>& layer) {
    auto itLayer = std::find(m_layers.begin(), m_layers.end(), layer);
    if(itLayer != m_layers.end()) {
        m_layers.erase(itLayer);
    }
}

void LayerStack::removeOverlayLayer(const std::shared_ptr<Layer>& layer) {
    auto itLayer = std::find(m_uiLayers.begin(), m_uiLayers.end(), layer);
    if(itLayer != m_uiLayers.end()) {
        m_uiLayers.erase(itLayer);
    }
}

void LayerStack::onUpdate() {
    for(auto& layer : m_layers) {
        if(layer->isEnabled()) {
            layer->onUpdate();
        }
    }

    for(auto& layer : m_uiLayers) {
        if(layer->isEnabled()) {
            layer->onUpdate();
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(m_window->getWidth(), m_window->getHeight());

    for(auto& layer : m_layers) {
        if(layer->isEnabled()) {
            layer->onImGuiRender();
        }
    }

    for(auto& layer : m_uiLayers) {
        if(layer->isEnabled()) {
            layer->onImGuiRender();
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

void LayerStack::onEvent(Event& e) {
    for(auto layer = m_uiLayers.rbegin(); layer != m_uiLayers.rend(); ++layer) {
        (*layer)->onEvent(e);
    }
    for(auto layer = m_layers.rbegin(); layer != m_layers.rend(); ++layer) {
        (*layer)->onEvent(e);
    }
}

}
