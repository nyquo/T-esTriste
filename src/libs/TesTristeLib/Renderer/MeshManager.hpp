#pragma once

#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/Renderer/Mesh.hpp>

#include <memory>
#include <testristelib_export.h>
#include <unordered_map>

namespace TesTriste {

class TET_EXPORT MeshManager {
  public:
    using MeshID = uint32_t;

    MeshManager() = default;
    MeshManager(const MeshManager& other) = delete;
    MeshManager(MeshManager&& other) noexcept = delete;
    MeshManager& operator=(const MeshManager& other) = delete;
    MeshManager& operator=(MeshManager&& other) noexcept = delete;
    ~MeshManager() = default;

    static MeshID hashMeshId(const std::string& name) {
        std::hash<std::string> hasher;
        return static_cast<MeshID>(hasher(name));
    }

    MeshID addMesh(std::shared_ptr<Mesh> mesh, const std::string& name = "") {
        MeshID id = hashMeshId(name);
        if(m_meshes.contains(id)) {
            Logger::logWarning("Mesh with ID already exists, replacing it.");
        }
        m_meshes[id] = std::move(mesh);
        return id;
    }

    std::shared_ptr<Mesh> getMesh(std::string name) {
        MeshID id = hashMeshId(name);
        return getMesh(id);
    }

    std::shared_ptr<Mesh> getMesh(MeshID id) {
        auto it = m_meshes.find(id);
        if(it != m_meshes.end()) {
            return it->second;
        }
        Logger::logError("Mesh with given ID does not exist.");
        return {};
    }

  private:
    std::unordered_map<MeshID, std::shared_ptr<Mesh>> m_meshes;
};

}