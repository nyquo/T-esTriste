set(LibName Glad)

tet_add_library(${LibName}
    SOURCES
        src/glad.c
    HEADERS
        include/glad/glad.h
        include/KHR/khrplatform.h
    PUBLIC_INCLUDES
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/libs/Glad/include>
        $<INSTALL_INTERFACE:include/>
    PRIVATE_INCLUDES
    PUBLIC_LINKS
        glfw
    PRIVATE_LINKS
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS
    RESSOURCES
)
