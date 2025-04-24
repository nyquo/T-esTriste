# Create a new executable for T-esTriste
#
# Usage:
#   tet_add_executable(AppName
#       SOURCES [item1...]
#       [INCLUDES [item1...] ]
#       [LINKS [item1...] ]
#       [DEFINITIONS [item1 ...]]
#       [RESSOURCES [item1 ...]])
#
# SOURCES are the sources of the executable
# INCLUDES an optional list of include directories that are dependencies
# LINKS an optional list of targets/libraries to link with as dependencies
# DEFINITIONS an optional list of definitions to add to the target
# RESSOURCES an optinal list of ressources to copy next to the executable
#
function(tet_add_executable AppName)
    set(_options "")
    set(_singleValues)
    set(_multipleValues SOURCES INCLUDES LINKS DEFINITIONS RESSOURCES)

    cmake_parse_arguments(PARAM "${_options}" "${_singleValues}" "${_multipleValues}" ${ARGN})

    if(NOT AppName)
        message(FATAL_ERROR "You must provide the executable name in 'tet_add_executable'")
    endif()

    if(NOT PARAM_SOURCES)
        message(FATAL_ERROR "You must provide the executable SOURCES in 'tet_add_executable'")
    endif()

    add_executable(${AppName} ${PARAM_SOURCES} ${PARAM_RESSOURCES})

    target_link_libraries(${AppName} PRIVATE ${PARAM_LINKS})
    target_include_directories(${AppName} PRIVATE ${PARAM_INCLUDES})
    target_compile_definitions(${AppName} PRIVATE ${PARAM_DEFINITIONS})

    set_target_properties(${AppName} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)
    set_target_properties(${AppName} PROPERTIES CXX_CLANG_TIDY
        clang-tidy;
        -format-style='file';
        -header-filter=^${CMAKE_CURRENT_SOURCE_DIR};
    )
    
    # Inspired from https://discourse.cmake.org/t/copy-resources-to-build-folder/1010/5
    # Copy ressources next to executable
    if(PARAM_RESSOURCES)
        get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

        set(RESSOURCES_FOLDER ${AppName}Ressources)
        target_compile_definitions(${AppName} PRIVATE RESSOURCES_FOLDER="${RESSOURCES_FOLDER}")
        set(COPY_TO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}$<$<BOOL:${GENERATOR_IS_MULTI_CONFIG}>:/$<CONFIG>>/${RESSOURCES_FOLDER}")

        foreach(RESSOURCE ${PARAM_RESSOURCES})
            set(DEST_FILE "${COPY_TO}/${RESSOURCE}")

            add_custom_command(
                OUTPUT "${DEST_FILE}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${CMAKE_CURRENT_SOURCE_DIR}/${RESSOURCE}" "${DEST_FILE}"
                COMMENT "Copying resource: ${RESSOURCE} for ${AppName}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${RESSOURCE}"
            )
            list(APPEND RESOURCE_OUTPUTS "${DEST_FILE}")
        endforeach()

        add_custom_target(${AppName}-ressources
            DEPENDS ${RESOURCE_OUTPUTS}
        )
        add_dependencies(${AppName} ${AppName}-ressources)
    endif()

endfunction()


# Create a new library for T-esTriste
#
# Usage:
#   tet_add_executable(LibraryName
#       SOURCES [item1...]
#       HEADERS [item1...]
#       [PUBLIC_INCLUDES [item1...]]
#       [PRIVATE_INCLUDES [item1...]]
#       [PUBLIC_LINKS [item1...]]
#       [PRIVATE_LINKS [item1...]]
#       [PUBLIC_DEFINITIONS [item1...]]
#       [PRIVATE_DEFINITIONS [item1...]]
#       [RESSOURCES [item1 ...]])
#
# SOURCES are the sources of the library
# HEADERS are the headers files associated to the library
# PUBLIC_INCLUDES an optional list of include directories that are public dependencies
# PRIVATE_INCLUDES an optional list of include directories that are private dependencies
# PUBLIC_LINKS an optional list of targets/libraries to link with as public dependencies
# PRIVATE_LINKS an optional list of targets/libraries to link with as private dependencies
# PUBLIC_DEFINITIONS an optional list of public definitions to add to the target
# PRIVATE_DEFINITIONS an optional list of private definitions to add to the target
# RESSOURCES an optinal list of ressources to copy next to the executable
#
function(tet_add_library LibraryName)
    set(_options "")
    set(_singleValues)
    set(_multipleValues SOURCES HEADERS PUBLIC_INCLUDES PRIVATE_INCLUDES PUBLIC_LINKS PRIVATE_LINKS PUBLIC_DEFINITIONS PRIVATE_DEFINITIONS RESSOURCES)

    cmake_parse_arguments(PARAM "${_options}" "${_singleValues}" "${_multipleValues}" ${ARGN})

    if(NOT LibraryName)
        message(FATAL_ERROR "You must provide the library name in 'tet_add_library'")
    endif()

    if(NOT PARAM_SOURCES AND NOT PARAM_HEADERS)
        message(FATAL_ERROR "You must provide the library SOURCES and/or HEADERS in 'tet_add_librar'")
    endif()

    if(NOT PARAM_SOURCES)
        add_library(${LibraryName} INTERFACE ${PARAM_SOURCES} ${PARAM_HEADERS} ${PARAM_RESSOURCES})
        target_link_libraries(${LibraryName} INTERFACE ${PARAM_PUBLIC_LINKS} ${PARAM_PRIVATE_LINKS})
        target_include_directories(${LibraryName} INTERFACE ${PARAM_PUBLIC_INCLUDES} ${PARAM_PRIVATE_INCLUDES})
    else()
        add_library(${LibraryName} ${PARAM_SOURCES} ${PARAM_HEADERS} ${PARAM_RESSOURCES})
        target_link_libraries(${LibraryName} PUBLIC ${PARAM_PUBLIC_LINKS} PRIVATE ${PARAM_PRIVATE_LINKS})
        target_include_directories(${LibraryName} PUBLIC ${PARAM_PUBLIC_INCLUDES} PRIVATE ${PARAM_PRIVATE_INCLUDES})
        target_compile_definitions(${LibraryName} PUBLIC ${PARAM_PUBLIC_DEFINITIONS} PRIVATE ${PARAM_PRIVATE_DEFINITIONS})
    endif()

    set_target_properties(${LibraryName} PROPERTIES CXX_CLANG_TIDY
        clang-tidy;
        -format-style='file';
        -header-filter=^${CMAKE_CURRENT_SOURCE_DIR};
    )

    # Copy ressources next to executable
    if(PARAM_RESSOURCES)
        get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

        set(RESSOURCES_FOLDER ${LibraryName}Ressources)
        target_compile_definitions(${LibraryName} PRIVATE RESSOURCES_FOLDER="${RESSOURCES_FOLDER}")
        set(COPY_TO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}$<$<BOOL:${GENERATOR_IS_MULTI_CONFIG}>:/$<CONFIG>>/${RESSOURCES_FOLDER}")

        foreach(RESSOURCE ${PARAM_RESSOURCES})
            set(DEST_FILE "${COPY_TO}/${RESSOURCE}")

            add_custom_command(
                OUTPUT "${DEST_FILE}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${CMAKE_CURRENT_SOURCE_DIR}/${RESSOURCE}" "${DEST_FILE}"
                COMMENT "Copying resource: ${RESSOURCE} for ${LibraryName}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${RESSOURCE}"
            )
            list(APPEND RESOURCE_OUTPUTS "${DEST_FILE}")
        endforeach()

        add_custom_target(${LibraryName}-ressources
            DEPENDS ${RESOURCE_OUTPUTS}
        )
        add_dependencies(${LibraryName} ${LibraryName}-ressources)
    endif()

endfunction()
