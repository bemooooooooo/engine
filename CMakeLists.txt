cmake_minimum_required(VERSION 3.15)

# ============================================================================
# Project definition
# ============================================================================
project(
    MyGameEngine
    LANGUAGES CXX C
    VERSION 0.1.0)

# ============================================================================
# Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)


# ============================================================================
# Compiler-specific settings
# ============================================================================

# Store original warning flags
if(MSVC)
    set(ORIGINAL_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(ORIGINAL_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

# Main project warnings (strict)
if(MSVC)
    add_compile_options(/W4)  # /WX = warnings as errors
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Save current compile options for restoration
get_directory_property(STRICT_COMPILE_OPTIONS COMPILE_OPTIONS)

# ============================================================================
# Project definitions
# ============================================================================
add_compile_definitions(ENGINE_DEBUG_BUILD=$<IF:$<CONFIG:Debug>,1,0>)

# ============================================================================
# Find packages
# ============================================================================
find_package(OpenGL REQUIRED)

# ============================================================================
# Helper function for external libraries
# ============================================================================
function(disable_warnings)
    # Clear strict warnings temporarily
    set_directory_properties(PROPERTIES COMPILE_OPTIONS "")
    if(MSVC)
        add_compile_options(/W1)
    else()
        # Disable all warnings but explicitly allow language extensions used in generated code
        add_compile_options(-w -Wno-language-extension-token)
    endif()
endfunction()

function(restore_warnings)
    # Clear previous options and restore strict warnings
    set_directory_properties(PROPERTIES COMPILE_OPTIONS "")
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

# ============================================================================
# External libraries setup
# ============================================================================

# STB Image Library
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${PROJECT_SOURCE_DIR}/external/stb)

# GLFW

disable_warnings()
add_subdirectory(external/glfw)
restore_warnings()

disable_warnings()
set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad_gl_core_mx_33 REPRODUCIBLE API gl:core=3.3)
restore_warnings()

# Assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

disable_warnings()
add_subdirectory(external/assimp)
restore_warnings()

# ============================================================================
# Main executable
# ============================================================================

file(GLOB_RECURSE ENGINE_SOURCES
    "src/Core/*.cpp"
    "src/Graphics/*.cpp"
    "src/Math/*.cpp"
    "src/Input/*.cpp"
)

add_executable(MyGameEngine
    src/main.cpp
    ${ENGINE_SOURCES}
)

# ============================================================================
# Target configuration
# ============================================================================

target_include_directories(MyGameEngine PRIVATE
    src/
    external/
    ${CMAKE_CURRENT_BINARY_DIR}/gladsources/glad_gl_core_mx_33/include
    ${ASSIMP_INCLUDE_DIRS}
)

target_link_libraries(MyGameEngine
    OpenGL::GL
    glfw
    glad_gl_core_mx_33
    stb_image
    assimp
)

# ============================================================================
# Post-build setup
# ============================================================================

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})