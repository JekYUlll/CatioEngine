//
// Created by JekYUlll on 2024/9/28.
//
#include "Graphic/CtVKSwapchain.h"

#include <Graphic/CtVKGraphicContext.h>

#include "Graphic/CtVKDevice.h"

namespace cte {
    CtVKSwapchain::CtVKSwapchain(CtVKGraphicContext *context, CtVKDevice *device)
        : m_handle(VK_NULL_HANDLE), m_context(context), m_device(device) {
        ReCreate();

    }

    CtVKSwapchain::~CtVKSwapchain() {
        vkDestroySwapchainKHR(m_device->GetHandle(), this->m_handle, nullptr);
        // VK_D(SwapchainKHR, m_device->GetHandle(), m_handle);
    }

    bool CtVKSwapchain::ReCreate() {
        LOG_D("-----------------------------");
        SetupSurfaceCapabilities();
        LOG_D("currentExtent : {0} x {1}", m_surfaceInfo.capabilities.currentExtent.width, m_surfaceInfo.capabilities.currentExtent.height);
        LOG_D("surfaceFormat : {0}", vk_format_string(m_surfaceInfo.surfaceFormat.format));
        LOG_D("presentMode   : {0}", vk_present_mode_string(m_surfaceInfo.presentMode));
        LOG_D("-----------------------------");

        uint32_t imageCount = m_device->GetSettings().swapChainImageCount;
        if(imageCount < m_surfaceInfo.capabilities.minImageCount && m_surfaceInfo.capabilities.minImageCount > 0) {
            imageCount = m_surfaceInfo.capabilities.minImageCount;
        }
        if(imageCount > m_surfaceInfo.capabilities.maxImageCount && m_surfaceInfo.capabilities.maxImageCount > 0) {
            imageCount = m_surfaceInfo.capabilities.maxImageCount;
        }

        VkSharingMode imageSharingMode;
        uint32_t queueFamilyIndexCount;
        uint32_t pQueueFamilyIndices[2] = { 0, 0 };
        if(m_context->IsSameGraphicPresentQueueFamily()) {
            imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            queueFamilyIndexCount = 0;
        } else {
            imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            queueFamilyIndexCount = 2;
            pQueueFamilyIndices[0] = m_context->GetGraphicQueueFamilyInfo().queueFamilyIndex;
            pQueueFamilyIndices[1] = m_context->GetPresentQueueFamilyInfo().queueFamilyIndex;
        }

        VkSwapchainKHR oldSwapChain = this->m_handle;

        VkSwapchainCreateInfoKHR swapChainCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = m_context->GetSurface(),
            .minImageCount = imageCount,
            .imageFormat = m_surfaceInfo.surfaceFormat.format,
            .imageColorSpace = m_surfaceInfo.surfaceFormat.colorSpace,
            .imageExtent = m_surfaceInfo.capabilities.currentExtent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = imageSharingMode,
            .queueFamilyIndexCount = queueFamilyIndexCount,
            .pQueueFamilyIndices = pQueueFamilyIndices,
            .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = m_surfaceInfo.presentMode,
            .clipped = VK_FALSE,
            .oldSwapchain = oldSwapChain
        };
        VkResult result = vkCreateSwapchainKHR(m_device->GetHandle(), &swapChainCreateInfo, nullptr, &m_handle);
        if(result != VK_SUCCESS) {
            LOG_E("{0} : {1}", __FUNCTION__, vk_result_string(result));
            return false;
        }
        LOG_T("Swapchain {0} : old: {1}, new: {2}, image count: {3}, format: {4}, present mode : {5}", __FUNCTION__, reinterpret_cast<void *>(oldSwapChain), reinterpret_cast<void *>(m_handle), imageCount,
              vk_format_string(m_surfaceInfo.surfaceFormat.format), vk_present_mode_string(m_surfaceInfo.presentMode));

        uint32_t swapchainImageCount = 0;
        result = vkGetSwapchainImagesKHR(m_device->GetHandle(), m_handle, &swapchainImageCount, nullptr);
        m_images.resize(swapchainImageCount);
        result = vkGetSwapchainImagesKHR(m_device->GetHandle(), m_handle, &swapchainImageCount, m_images.data());
        // @@@ 销毁旧的？
        vkDestroySwapchainKHR(m_device->GetHandle(), oldSwapChain, nullptr);
        return result == VK_SUCCESS;
    }

    void CtVKSwapchain::SetupSurfaceCapabilities() {
        // capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_context->GetPhysicalDevice(), m_context->GetSurface(), &m_surfaceInfo.capabilities);

        CtVkSettings settings  = m_device->GetSettings();

        // format (surface)
        uint32_t formatCount;
        CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(m_context->GetPhysicalDevice(), m_context->GetSurface(), &formatCount, nullptr));
        if(formatCount == 0) {
            LOG_E("{0} : num of surface format is 0", __FUNCTION__);
            return;
        }
        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(m_context->GetPhysicalDevice(), m_context->GetSurface(), &formatCount, formats.data()));
        int32_t foundFormatIndex = -1;
        for(size_t i = 0; i < formatCount; i++) {
            if(formats[i].format == settings.surfaceFormat && formats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
                foundFormatIndex = i;
                break;
            }
        }
        if(foundFormatIndex == -1) { foundFormatIndex = 0; }
        m_surfaceInfo.surfaceFormat = formats[foundFormatIndex];

        // present mode
        uint32_t presentModeCount;
        CALL_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(m_context->GetPhysicalDevice(), m_context->GetSurface(), &presentModeCount, nullptr));
        if(presentModeCount == 0) {
            LOG_E("{0} : num of present mod is 0", __FUNCTION__);
            return;
        }
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        CALL_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(m_context->GetPhysicalDevice(), m_context->GetSurface(), &presentModeCount, presentModes.data()));
        const VkPresentModeKHR preferredPresentMode = m_device->GetSettings().presentMode;
        int32_t foundPresentModeIndex = -1;
        for(size_t i = 0; i < presentModeCount; i++) {
            if(presentModes[i] == preferredPresentMode) {
                foundPresentModeIndex = i;
                break;
            }
        }
        if(foundPresentModeIndex >= 0) {
            m_surfaceInfo.presentMode = presentModes[foundPresentModeIndex];
        } else {
            m_surfaceInfo.presentMode = presentModes[0];
        }
    }

}
