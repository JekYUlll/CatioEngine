//
// Created by JekYUlll on 2024/9/28.
//

#ifndef CTVKSWAPCHAIN_H
#define CTVKSWAPCHAIN_H
#include <vector>
#include "CtVKCommon.h"

namespace cte {

    class CtVKGraphicContext;
    class CtVKDevice;

    struct SurfaceInfo {
        VkSurfaceCapabilitiesKHR capabilities;
        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;
    };

    // 交换链
    class CtVKSwapchain {
    public:
        CtVKSwapchain(CtVKGraphicContext* context, CtVKDevice* device);
        ~CtVKSwapchain();

        bool ReCreate();

    private:
        void SetupSurfaceCapabilities();

        VkSwapchainKHR m_handle;
        CtVKGraphicContext* m_context;
        CtVKDevice* m_device;
        std::vector<VkImage> m_images;

        SurfaceInfo m_surfaceInfo;

    };

}


#endif //CTVKSWAPCHAIN_H