cmake_minimum_required(VERSION 3.22)

# shader files
set(SHADERS
        00_hello_triangle.vert
        00_hello_triangle.frag
)
spirv_shaders(
        SHADER_HEADERS
        SPIRV_VERSION
        1.0 # Currently targeting Vulkan 1.0
        SOURCES
        ${SHADERS}
)

add_executable(SandBox
        ${SHADER_HEADERS}
        Main.cpp
)

target_link_libraries(SandBox PRIVATE catio_core)

