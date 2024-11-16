cmake_minimum_required(VERSION 3.22)

add_library(catio_platform
        Private/CtLog.cpp
        Private/CtWindow.cpp
        Private/Window/CtGLFWwindow.cpp
        Private/CtGraphicContext.cpp
        Private/Graphic/CtVKGraphicContext.cpp
        Private/Graphic/CtVKDevice.cpp
        Private/Graphic/CtVKQueue.cpp
        Public/Graphic/CtVKSwapchain.h
        Private/Graphic/CtVKSwapchain.cpp
        Public/Graphic/CtVKRenderPass.h
        Private/Graphic/CtVKRenderPass.cpp
)

target_include_directories(catio_platform PUBLIC External)

# glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(External/glfw)
target_link_libraries(catio_platform PRIVATE glfw)

# glm
add_subdirectory(External/glm)
target_link_libraries(catio_platform PRIVATE glm)

# Vulkan
find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
    message("----> Find vulkan success : ${Vulkan_INCLUDE_DIRS}")
endif()
target_include_directories(catio_platform PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(catio_platform PRIVATE ${Vulkan_LIBRARY})