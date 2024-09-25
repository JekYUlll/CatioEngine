//
// Created by JekYUlll on 2024/9/24.
//

#ifndef CTGLFWWINDOW_H
#define CTGLFWWINDOW_H
#include "GLFW/glfw3.h"
#include <GLFW/glfw3native.h>
#include <CtWindow.h>
#include <CtLog.h>

namespace cte {

    class CtGLFWwindow : public CtWindow {
    public:
        CtGLFWwindow() = delete;
        CtGLFWwindow(uint32_t width, uint32_t height, const char* title);
        ~CtGLFWwindow() override;

        bool ShouldClose() override;
        void PollEvents() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_GLFWwindow;
    };

}

#endif //CTGLFWWINDOW_H
