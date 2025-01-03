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
        uint32_t swapChainImageCount = 3;
    };

    // 逻辑设备
    class CtVKDevice {
    public:
        CtVKDevice(CtVKGraphicContext* context, uint32_t graphicQueueCount, uint32_t presentQueueCount, const CtVkSettings& settings = {});
        ~CtVKDevice();

        [[nodiscard]] VkDevice GetHandle() const { return this->m_handle; }
        [[nodiscard]] const CtVkSettings& GetSettings() const { return this->m_settings; }
        [[nodiscard]] VkPipelineCache GetPipelineCache() const { return this->m_pipelineCache; }

    private:
        void CreatePipelineCache();

        VkDevice m_handle;

        std::vector<std::shared_ptr<CtVKQueue>> m_graphicQueues;
        std::vector<std::shared_ptr<CtVKQueue>> m_presentQueues;

        CtVkSettings m_settings;

        VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;
    };

}

#endif //CTVKDEVICE_H
