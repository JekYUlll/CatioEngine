//
// Created by JekYUlll on 2024/9/28.
//
#include <vector>
#include "Graphic/CtVKDevice.h"
#include "Graphic/CtVKGraphicContext.h"
#include "Graphic/CtVKQueue.h"
#include "CtLog.h"

namespace cte {

    constexpr DeviceFeature requestedExtensions[] = {
        { VK_KHR_SWAPCHAIN_EXTENSION_NAME, true }, // 交换链的扩展
#ifdef CT_ENGINE_PLATFORM_WIN32
#elif CT_ENGINE_PLATFORM_MACOS
        { "VK_KHR_portability_subset", true },
#elif CT_ENGINE_PLATFORM_LINUX
#endif
    };

    CtVKDevice::CtVKDevice(CtVKGraphicContext *context, uint32_t graphicQueueCount, uint32_t presentQueueCount,
        const CtVkSettings &settings) : m_handle(VK_NULL_HANDLE) {
        if(!context) {
            LOG_E("Must create a vulkan context before creating device.");
            return;
        }

        const QueueFamilyInfo graphicQueueFamilyInfo = context->GetGraphicQueueFamilyInfo();
        const QueueFamilyInfo presentQueueFamilyInfo = context->GetPresentQueueFamilyInfo();

        if(graphicQueueCount > graphicQueueFamilyInfo.queueCount) {
            LOG_E("this graphic queue family has {0} queue, but request {1}", graphicQueueFamilyInfo.queueCount, graphicQueueCount);
            return;
        }
        if(presentQueueCount > presentQueueFamilyInfo.queueCount) {
            LOG_E("this present queue family has {0} queue, but request {1}", presentQueueFamilyInfo.queueCount, presentQueueCount);
            return;
        }
        std::vector<float> graphicQueuePriorities(graphicQueueCount, 0.0f);
        std::vector<float> presentQueuePriorities(presentQueueCount, 1.0f);

        bool bSameQueueFamilyIndex = context->IsSameGraphicPresentQueueFamily();
        uint32_t sameQueueCount = graphicQueueCount;
        if(bSameQueueFamilyIndex) {
            sameQueueCount += presentQueueCount;
            if(sameQueueCount > graphicQueueFamilyInfo.queueCount) {
                sameQueueCount = graphicQueueFamilyInfo.queueCount;
            }
            graphicQueuePriorities.insert(graphicQueuePriorities.begin(), presentQueuePriorities.begin(),
                                          presentQueuePriorities.end());
        }


        VkDeviceQueueCreateInfo queueInfo[2] = {
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = static_cast<uint32_t>(graphicQueueFamilyInfo.queueFamilyIndex),
                .queueCount = sameQueueCount,
                .pQueuePriorities = graphicQueuePriorities.data()
            }
        };

        if(!bSameQueueFamilyIndex) {
            queueInfo[1] = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = static_cast<uint32_t>(presentQueueFamilyInfo.queueFamilyIndex),
                .queueCount = presentQueueCount,
                .pQueuePriorities = presentQueuePriorities.data()
            };
        }

        uint32_t availableExtensionCount = 0;
        CALL_VK(vkEnumerateDeviceExtensionProperties(context->GetPhysicalDevice(), "", &availableExtensionCount, nullptr));
        std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
        CALL_VK(vkEnumerateDeviceExtensionProperties(context->GetPhysicalDevice(), "", &availableExtensionCount, availableExtensions.data()));
        uint32_t enabledExtensionCount = 0;
        const char* enableExtensions[32];
        if(!checkDeviceFeatures("Device Extensions", true, availableExtensionCount, availableExtensions.data(),
                            ARRAY_SIZE(requestedExtensions), requestedExtensions, &enabledExtensionCount, enableExtensions)) {
            return;
        }

        const VkDeviceCreateInfo deviceInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = static_cast<uint32_t>(bSameQueueFamilyIndex ? 1 : 2),
            .pQueueCreateInfos = queueInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = enabledExtensionCount,
            .ppEnabledExtensionNames = enabledExtensionCount > 0 ? enableExtensions : nullptr,
            .pEnabledFeatures = nullptr
        };
        CALL_VK(vkCreateDevice(context->GetPhysicalDevice(), &deviceInfo, nullptr, &(this->m_handle)));
        LOG_T("VkDevice: {0}", static_cast<void *>(m_handle));

        for(int i = 0; i < graphicQueueCount; i++) {
            VkQueue queue;
            vkGetDeviceQueue(this->m_handle, graphicQueueFamilyInfo.queueFamilyIndex, i , &queue);
            this->m_graphicQueues.push_back(std::make_shared<CtVKQueue>(graphicQueueFamilyInfo.queueFamilyIndex, i, queue, false));
        }
        for(int i = 0; i < presentQueueCount; i++) {
            VkQueue queue;
            vkGetDeviceQueue(this->m_handle, presentQueueFamilyInfo.queueFamilyIndex, i , &queue);
            this->m_presentQueues.push_back(std::make_shared<CtVKQueue>(presentQueueFamilyInfo.queueFamilyIndex, i, queue, true));
        }
        // create a pipeline cache
        CreatePipelineCache();
    }

    CtVKDevice::~CtVKDevice() {
        vkDeviceWaitIdle(this->m_handle);
        VK_D(PipelineCache, m_handle, m_pipelineCache);
        vkDestroyDevice(this->m_handle, nullptr);
    }

    void CtVKDevice::CreatePipelineCache() {
        VkPipelineCacheCreateInfo pipelineCacheInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
        };
        CALL_VK(vkCreatePipelineCache(this->m_handle, &pipelineCacheInfo, nullptr, &m_pipelineCache));
    }
}
