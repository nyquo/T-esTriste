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

    MeshID addMesh(std::unique_ptr<Mesh> mesh) {
        static MeshID nextID = 0;
        nextID++;
        m_meshes[nextID] = std::move(mesh);
        return nextID;
    }

    Mesh& getMesh(MeshID id) {
        auto it = m_meshes.find(id);
        if(it != m_meshes.end()) {
            return *(it->second);
        }
        Logger::logError("Mesh with given ID does not exist.");
    }

  private:
    std::unordered_map<MeshID, std::unique_ptr<Mesh>> m_meshes;
};

}