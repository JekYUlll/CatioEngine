//
// Created by JekYUlll on 2024/9/24.
//

#ifndef CTWINDOW_H
#define CTWINDOW_H

#include "CtEngine.h"
// #include "glfw/src/internal.h" 引入会链接错误。傻逼Clion

namespace cte {
    class CtWindow {
    public:
        CtWindow(const CtWindow&) = delete;
        CtWindow& operator=(const CtWindow&) = delete;
        virtual ~CtWindow() = default;

        static std::unique_ptr<CtWindow> Create(uint32_t width, uint32_t height, const char *title);

        virtual bool ShouldClose() = 0;
        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;

    protected:
        CtWindow() = default;

    };
}

#endif //CTWINDOW_H
