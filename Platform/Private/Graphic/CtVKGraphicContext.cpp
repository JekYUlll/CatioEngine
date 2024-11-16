//
// Created by JekYUlll on 2024/9/25.
//
#include <vector>
#include "Graphic/CtVKGraphicContext.h"

#include <unordered_set>

#include "Window/CtGLFWwindow.h"

namespace cte {

    constexpr DeviceFeature requestedLayers[] =  {
        { "VK_LAYER_KHRONOS_validation", true },
        {"TEST_layers", false}
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
        CreateSurface(window);
        SelectPhysicalDevice();
    }

    CtVKGraphicContext::~CtVKGraphicContext() {
        vkDestroySurfaceKHR(this->m_instance, this->m_surface, nullptr);
        vkDestroyInstance(this->m_instance, nullptr);
    }

    // 添加 VKAPI_PTR 防止 msvc 报错
    static VkBool32 VKAPI_PTR VkDebugReportCallback(VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT                  objectType,
                uint64_t                                    object,
                size_t                                      location,
                int32_t                                     messageCode,
                const char*                                 pLayerPrefix,
                const char*                                 pMessage,
                void*                                       pUserData) {
        if(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT){
            LOG_E("{0}", pMessage);
        }
        if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT){
            LOG_W("{0}", pMessage);
        }
        return VK_TRUE;
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
        if(this->bShouldValidate) {
            if(!checkDeviceFeatures("Instance Layers", false, availableLayerCount, availableLayers.data(),
                                    ARRAY_SIZE(requestedLayers), requestedLayers, &enabledLayerCount, enableLayers)) {
                return;
            }
        }

        // 2. 构建extension
        uint32_t availableExtensionCount = 0;
        CALL_VK(vkEnumerateInstanceExtensionProperties("", &availableExtensionCount, nullptr));
        std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
        CALL_VK(vkEnumerateInstanceExtensionProperties("", &availableExtensionCount, availableExtensions.data()));

        uint32_t glfwRequestedExtensionCount = 0;
        const char** glfwRequestedExtensions = glfwGetRequiredInstanceExtensions(&glfwRequestedExtensionCount);
        std::unordered_set<const char*> allRequestedExtensionsSet; // 所有的扩展
        std::vector<DeviceFeature> allRequestedExtensions;
        for(const auto& extension : requestedExtensions) {
            if(allRequestedExtensionsSet.find(extension.name) == allRequestedExtensionsSet.end()) {
                allRequestedExtensionsSet.insert(extension.name);
                allRequestedExtensions.push_back(extension);
            }
        }
        for(int i = 0; i < glfwRequestedExtensionCount; i++) {
            const char* extensionName = glfwRequestedExtensions[i];
            if(allRequestedExtensionsSet.find(extensionName) == allRequestedExtensionsSet.end()) {
                allRequestedExtensionsSet.insert(extensionName);
                allRequestedExtensions.push_back({extensionName, true});
            }
        }

        uint32_t enabledExtensionCount = 0;
        const char* enableExtensions[32];
        if(!checkDeviceFeatures("Instance Extensions", true, availableExtensionCount, availableExtensions.data(),
                            allRequestedExtensions.size(), allRequestedExtensions.data(), &enabledExtensionCount, enableExtensions)) {
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

        VkDebugReportCallbackCreateInfoEXT debugReportCallbackInfoExt = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = VK_DEBUG_REPORT_WARNING_BIT_EXT
                    | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
                    | VK_DEBUG_REPORT_ERROR_BIT_EXT,
            .pfnCallback = VkDebugReportCallback
        };

        // 3. create instance
        VkInstanceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = this->bShouldValidate ? &debugReportCallbackInfoExt : nullptr,
            .flags = 0,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = enabledLayerCount,
            .ppEnabledLayerNames = enabledLayerCount > 0 ? enableLayers : nullptr,
            .enabledExtensionCount = enabledExtensionCount,
            .ppEnabledExtensionNames = enabledExtensionCount > 0 ? enableExtensions : nullptr,
        };
        CALL_VK(vkCreateInstance(&createInfo, nullptr, &(this->m_instance)));
        if(!m_instance) {
            LOG_E("instance creation failed");
        } else {
            LOG_T("{0} : instance : {1}", __FUNCTION__, static_cast<void *>(m_instance));
        }
    }

    void CtVKGraphicContext::CreateSurface(CtWindow* window) {
        if(!window) {
            LOG_E("window not exits.");
            return;
        }
        auto* glfwWindow = dynamic_cast<CtGLFWwindow *>(window);
        if(!glfwWindow) {
            // to do
            LOG_E("not a glfw window.");
            return;
        }
        CALL_VK(glfwCreateWindowSurface(m_instance, glfwWindow->GetGLFWwindowHandle(), nullptr, &(this->m_surface)));
        LOG_T("{0} : surface: {1}", __FUNCTION__, reinterpret_cast<void *>(this->m_surface));
    }

    void CtVKGraphicContext::SelectPhysicalDevice() {
        uint32_t physicalDeviceCount = 0;
        CALL_VK(vkEnumeratePhysicalDevices(this->m_instance, &physicalDeviceCount, nullptr));
        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount); // 为什么会有重复？是否需要set降重？
        CALL_VK(vkEnumeratePhysicalDevices(this->m_instance, &physicalDeviceCount, physicalDevices.data()));

        uint32_t maxScore = 0;
        int32_t maxScorePhysicalDeviceIndex = -1;
        LOG_D("-----------------------------");
        LOG_D("Physical devices: ");
        for(int i = 0; i < physicalDeviceCount; i++) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);
            PrintPhysicalDeviceInfo(properties);

            uint32_t score = GetPhysicalDeviceScore(properties);
            uint32_t formatCount = 0; // 硬件支持的格式
            CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[i], m_surface, &formatCount, nullptr));
            std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
            CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[i], m_surface, &formatCount, surfaceFormats.data()));
            for(int j = 0; j < formatCount; j++){
                if(surfaceFormats[j].format == VK_FORMAT_B8G8R8A8_UNORM && surfaceFormats[j].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR){
                    score += 10;
                    break;
                }
            }
            // query queue family (查询物理设备队列族)
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilies.data());

            LOG_D("score    --->    : {0}", score);
            LOG_D("queue family     : {0}", queueFamilyCount);
            if(score < maxScore) {
                continue;
            }
            for(int j = 0; j < queueFamilyCount; j++) {
                if(queueFamilies[j].queueCount == 0) {
                    continue;
                }
                // 1. graphic family 图形渲染管线
                if(queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    this->m_graphicQueueFamily.queueFamilyIndex = j;
                    this->m_graphicQueueFamily.queueCount = queueFamilies[j].queueCount;
                }
                if(this->m_graphicQueueFamily.queueFamilyIndex >= 0 && m_presentQueueFamily.queueFamilyIndex >= 0
                    && this->m_graphicQueueFamily.queueFamilyIndex != this->m_presentQueueFamily.queueFamilyIndex) {
                    break;
                }
                // 2. present family 显示
                VkBool32 bSupportSurface = false; // 是否支持surface，支持则为显示设备
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[i], j, this->m_surface, &bSupportSurface);
                if(bSupportSurface) {
                    this->m_presentQueueFamily.queueFamilyIndex = j;
                    this->m_presentQueueFamily.queueCount = queueFamilies[j].queueCount;
                }
            }
            if(this->m_graphicQueueFamily.queueFamilyIndex >= 0 && this->m_presentQueueFamily.queueFamilyIndex >= 0) {
                maxScorePhysicalDeviceIndex = i;
                maxScore = score;
            }
        }
        LOG_D("-----------------------------");

        if(maxScorePhysicalDeviceIndex < 0) {
            LOG_W("Maybe can not find a suitable physical device, will use 0.");
            maxScorePhysicalDeviceIndex = 0;
        }
        this->m_physicalDevice =  physicalDevices[maxScorePhysicalDeviceIndex];
        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &(this->m_memoryProperties));
        LOG_T("{0} : physical device:{1}, score:{2}, graphic queue: {3} : {4}, present queue: {5} : {6}",
              __FUNCTION__, maxScorePhysicalDeviceIndex, maxScore,
              m_graphicQueueFamily.queueFamilyIndex, m_graphicQueueFamily.queueCount,
              m_presentQueueFamily.queueFamilyIndex, m_presentQueueFamily.queueCount);
        // 上面有问题。present queue: 4294967295 : 0，m_presentQueueFamily.queueFamilyIndex直接UINT32_MAX了。发现是写成了unsigned，又赋值为-1
    }

    void CtVKGraphicContext::PrintPhysicalDeviceInfo(VkPhysicalDeviceProperties& properties) {
        const char *deviceType = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "integrated gpu" :
                                 properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "discrete gpu" :
                                 properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? "virtual gpu" :
                                 properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "cpu" : "others";

        uint32_t driverVersionMajor = VK_VERSION_MAJOR(properties.driverVersion);
        uint32_t driverVersionMinor = VK_VERSION_MINOR(properties.driverVersion);
        uint32_t driverVersionPatch = VK_VERSION_PATCH(properties.driverVersion);

        uint32_t apiVersionMajor = VK_VERSION_MAJOR(properties.apiVersion);
        uint32_t apiVersionMinor = VK_VERSION_MINOR(properties.apiVersion);
        uint32_t apiVersionPatch = VK_VERSION_PATCH(properties.apiVersion);

        LOG_D("-----------------------------");
        LOG_D("deviceName       : {0}", properties.deviceName);
        LOG_D("deviceType       : {0}", deviceType);
        LOG_D("vendorID         : {0}", properties.vendorID);
        LOG_D("deviceID         : {0}", properties.deviceID);
        LOG_D("driverVersion    : {0}.{1}.{2}", driverVersionMajor, driverVersionMinor, driverVersionPatch);
        LOG_D("apiVersion       : {0}.{1}.{2}", apiVersionMajor, apiVersionMinor, apiVersionPatch);
    }

    uint32_t CtVKGraphicContext::GetPhysicalDeviceScore(const VkPhysicalDeviceProperties &properties) {
        const VkPhysicalDeviceType deviceType = properties.deviceType;
        uint32_t score = 0;
        switch (deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                score += 40;
            break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                score += 30;
            break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                score += 20;
            break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                score += 10;
            break;
            case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
                break;
        }
        return score;
    }

}
