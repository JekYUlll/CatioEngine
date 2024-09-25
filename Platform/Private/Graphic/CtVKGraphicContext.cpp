//
// Created by JekYUlll on 2024/9/25.
//
#include <vector>
#include "Graphic/CtVKGraphicContext.h"

namespace cte {

    constexpr DeviceFeature requestedLayers[] =  {
        { "VK_LAYER_KHRONOS_validation", true }
    };

    constexpr DeviceFeature requestedExtensions[] = {
        { VK_KHR_SURFACE_EXTENSION_NAME, true },
#ifdef CT_ENGINE_PLATFORM_WIN32
        { VK_KHR_WIN32_SURFACE_EXTENSION_NAME, true },
#elif CT_ENGINE_PLATFORM_MACOS
        { VK_MVK_MACOS_SURFACE_EXTENSION_NAME, true },
#elif CT_ENGINE_PLATFORM_LINUX
        { VK_KHR_XCB_SURFACE_EXTENSION_NAME, true },
#endif
    { VK_EXT_DEBUG_REPORT_EXTENSION_NAME, true},
};

    CtVKGraphicContext::CtVKGraphicContext(CtWindow *window) {
        CreateInstance();
    }

    CtVKGraphicContext::~CtVKGraphicContext() {

    }

    void CtVKGraphicContext::CreateInstance() {
        // 1. 构建layers
        uint32_t availableLayerCount = 0;
        CALL_VK(vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr));
        // VkLayerProperties availableLayers[availableLayerCount]; // 弹幕：这里应该用vector吧，c++不支持可变长数组，up主应该是在mac下用的llvm的拓展编译成功的
        std::vector<VkLayerProperties> availableLayers(availableLayerCount);
        CALL_VK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data()));

        uint32_t enabledLayerCount = 0;
        const char* enableLayers[32];
        if(!checkDeviceFeatures("Instance Layers", false, availableLayerCount, availableLayers.data(),
                            ARRAY_SIZE(requestedLayers), requestedLayers, &enabledLayerCount, enableLayers)) {
            return;
        }

        // 2. 构建extension
        uint32_t availableExtensionCount = 0;
        CALL_VK(vkEnumerateInstanceExtensionProperties("", &availableExtensionCount, nullptr));
        std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
        CALL_VK(vkEnumerateInstanceExtensionProperties("", &availableExtensionCount, availableExtensions.data()));

        uint32_t enabledExtensionCount = 0;
        const char* enableExtensions[32];
        if(!checkDeviceFeatures("Instance Extensions", true, availableExtensionCount, availableExtensions.data(),
                            ARRAY_SIZE(requestedExtensions), requestedExtensions, &enabledExtensionCount, enableExtensions)) {
            return;
        }

        VkApplicationInfo applicationInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Catio_Engine",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "None",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_3
        };

        // 3. create instance
        VkInstanceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = enabledLayerCount,
            .ppEnabledLayerNames = enabledLayerCount > 0 ? enableLayers : nullptr,
            .enabledExtensionCount = enabledExtensionCount,
            .ppEnabledExtensionNames = enabledExtensionCount > 0 ? enableExtensions : nullptr,
        };
        CALL_VK(vkCreateInstance(&createInfo, nullptr, &(this->m_instance)));
        LOG_T("{0} : instance : {1}", (void*)m_instance); // ?
    }
}
