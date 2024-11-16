//
// Created by JekYUlll on 2024/9/28.
//

#ifndef CTVKDEVICE_H
#define CTVKDEVICE_H
#include <cstdint>
#include <vector>
#include "CtVKCommon.h"

namespace cte {

    class CtVKGraphicContext;
    class CtVKQueue;

    struct CtVkSettings {
        VkFormat surfaceFormat = VK_FORMAT_B8G8R8A8_UNORM;
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        uint32_t swapchainImageCount = 3;
    };

    // 逻辑设备
    class CtVKDevice {
    public:
        CtVKDevice(CtVKGraphicContext* context, uint32_t graphicQueueCount, uint32_t presentQueueCount, const CtVkSettings& settings = {});
        ~CtVKDevice();

        [[nodiscard]] inline VkDevice GetHandle() const { return this->m_handle; }
        [[nodiscard]] inline const CtVkSettings& GetSettings() const { return this->m_settings; }

    private:
        VkDevice m_handle;

        std::vector<std::shared_ptr<CtVKQueue>> m_graphicQueues;
        std::vector<std::shared_ptr<CtVKQueue>> m_presentQueues;

        CtVkSettings m_settings;
    };

}

#endif //CTVKDEVICE_H
