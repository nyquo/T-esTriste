#pragma once

#include <TesTristeLib/Core/Logger.hpp>

#include <filesystem>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT ProgramLocation {
  public:
    static std::filesystem::path getProgramLocation() {
        if(m_programLocation.empty()) {
            Logger::logError("Program location is not set. Please call setProgramLocation() first.");
            return {};
        }
        return m_programLocation;
    }

    static void setProgramLocation(const std::filesystem::path& path) {
        if(!std::filesystem::exists(path)) {
            Logger::logError("Provided path does not exist: ", path.string());
            return;
        }
        m_programLocation = path;
        Logger::logInfo("Program location set to: ", m_programLocation.string());
    }

  private:
    // TODO maybe add TET_EXPORT ?
    static std::filesystem::path m_programLocation;
};

}