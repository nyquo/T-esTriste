#pragma once

#include <TesTristeLib/Renderer/MeshManager.hpp>
#include <TesTristeLib/Renderer/ShaderManager.hpp>

#include <testristelib_export.h>

namespace TesTriste {

struct TET_EXPORT AppContext {
    AppContext() = default;
    AppContext(const AppContext& other) = delete;
    AppContext(AppContext&& other) noexcept = delete;
    AppContext& operator=(const AppContext& other) = delete;
    AppContext& operator=(AppContext&& other) noexcept = delete;

    ShaderManager shaderManager;
    MeshManager meshManager;
};

}