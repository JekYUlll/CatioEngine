//
// Created by JekYUlll on 2024/9/24.
//
#include "Window/CtGLFWwindow.h"

namespace cte {

    CtGLFWwindow::CtGLFWwindow(uint32_t width, uint32_t height, const char *title) {
        if(!glfwInit()) {
            LOG_E("Failed to initialize GLFW");
            exit(EXIT_FAILURE);
            return;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        this->m_GLFWwindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if(!this->m_GLFWwindow) {
            LOG_E("Failed to create GLFW window.");
            exit(EXIT_FAILURE);
            return;
        }

        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        if(primaryMonitor) {
            int xPos, yPos, workWidth, workHeight;
            glfwGetMonitorWorkarea(primaryMonitor, &xPos, &yPos, &workWidth, &workHeight);
            // 设置窗口初始化的时候居中
            glfwSetWindowPos(this->m_GLFWwindow, workWidth / 2 - width / 2, workHeight / 2 - height / 2);
        } else {
            LOG_E("Failed to get GLFW window.");
        }

        // 关闭垂直同步，Vulkan似乎没有这个问题(使用OpenGl的时候会自动开启垂直同步？)
        // glfwSwapInterval(0);

        glfwMakeContextCurrent(this->m_GLFWwindow);

        glfwShowWindow(this->m_GLFWwindow);

    }

    CtGLFWwindow::~CtGLFWwindow() {
        glfwDestroyWindow(this->m_GLFWwindow);
        glfwTerminate();
        LOG_D("Application running end.");
    }

    bool CtGLFWwindow::ShouldClose() {
        return glfwWindowShouldClose(m_GLFWwindow);
    }

    void CtGLFWwindow::PollEvents() {
        glfwPollEvents();
    }

    void CtGLFWwindow::SwapBuffers() {
        glfwSwapBuffers(m_GLFWwindow);
    }
}
