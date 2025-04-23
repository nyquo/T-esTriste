set(HEADERS
    Core/gl.hpp
    Core/Logger.hpp
)

set(SOURCES
    Core/Logger.cpp
)

Tet_add_library(TesTristeLib
    SOURCES
        ${SOURCES}
    HEADERS
        ${HEADERS}
    PUBLIC_INCLUDES
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/libs>
        $<INSTALL_INTERFACE:include/>
    PRIVATE_INCLUDES
    PUBLIC_LINKS
    PRIVATE_LINKS
        Glad
        Imgui
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS
    RESSOURCES
)
