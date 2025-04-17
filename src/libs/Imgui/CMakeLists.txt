set(HEADERS
    imconfig.h
    imgui_impl_glfw.h
    imgui_impl_opengl3_loader.h
    imgui_impl_opengl3.h
    imgui_internal.h
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

set(SOURCES
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
)

Tet_add_library(Imgui
    SOURCES
        ${SOURCES}
    HEADERS
        ${HEADERS}
    PUBLIC_INCLUDES
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/libs/Imgui>
        $<INSTALL_INTERFACE:include/>
    PRIVATE_INCLUDES
    PUBLIC_LINKS
    PRIVATE_LINKS
        glfw
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS
    RESSOURCES
)

# Disable clang tidy for Imgui as this source files comes from an other repo
set_target_properties(Imgui PROPERTIES CXX_CLANG_TIDY "")
