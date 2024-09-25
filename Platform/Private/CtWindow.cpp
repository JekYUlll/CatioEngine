//
// Created by JekYUlll on 2024/9/24.
//
#include "CtWindow.h"
#include "Window/CtGLFWwindow.h"

namespace cte {

    std::unique_ptr<CtWindow> CtWindow::Create(uint32_t width, uint32_t height, const char *title) {
#ifdef CT_ENGINE_PLATFORM_WIN32
        return std::make_unique<CtGLFWwindow>(width, height, title);
#elif CT_ENGINE_PLATFORM_LINUX
        return std::make_unique<CtGLFWwindow>(width, height, title);
#elif CT_ENGINE_PLATFORM_MACOS
        return std::make_unique<CtGLFWwindow>(width, height, title);
#endif
        return nullptr;
    }

}

