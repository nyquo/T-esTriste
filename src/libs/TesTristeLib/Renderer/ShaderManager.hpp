#pragma once

#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/Renderer/Shader.hpp>

#include <memory>
#include <testristelib_export.h>
#include <unordered_map>

namespace TesTriste {

class TET_EXPORT ShaderManager {
  public:
    using ShaderID = uint32_t;

    ShaderManager() = default;
    ShaderManager(const ShaderManager& other) = delete;
    ShaderManager(ShaderManager&& other) noexcept = delete;
    ShaderManager& operator=(const ShaderManager& other) = delete;
    ShaderManager& operator=(ShaderManager&& other) noexcept = delete;
    ~ShaderManager() = default;

    static ShaderID hashShaderId(const std::string& name) {
        std::hash<std::string> hasher;
        return static_cast<ShaderID>(hasher(name));
    }

    ShaderID addShader(std::unique_ptr<Shader> shader, const std::string& name = "") {
        ShaderID id = hashShaderId(name);
        if(m_shaders.contains(id)) {
            Logger::logWarning("Shader with ID already exists, replacing it.");
        }
        m_shaders[id] = std::move(shader);
        return id;
    }

    ShaderID addShader(const fs::path& vertexPath, const fs::path& fragmentPath, const std::string& name = "") {
        ShaderID id = hashShaderId(name);
        if(m_shaders.contains(id)) {
            Logger::logWarning("Shader with ID already exists, replacing it.");
        }
        m_shaders[id] = std::make_unique<Shader>(vertexPath, fragmentPath);
        return id;
    }

    Shader& getShader(ShaderID id) {
        auto it = m_shaders.find(id);
        if(it != m_shaders.end()) {
            return *(it->second);
        }
        Logger::logError("Shader with given ID does not exist.");
    }

  private:
    std::unordered_map<ShaderID, std::unique_ptr<Shader>> m_shaders;
};

}