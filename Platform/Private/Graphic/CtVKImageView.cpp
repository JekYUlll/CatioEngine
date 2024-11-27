//
// Created by JekYUlll on 2024/11/27.
//

#include "Graphic/CtVKImageView.h"
#include "Graphic/CtVKDevice.h"

namespace cte {

    CtVKImageView::CtVKImageView(CtVKDevice *device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
        : m_device(device) {
        VkImageViewCreateInfo imageViewInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = format,
            .components = {
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask = aspectFlags,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            }
        };
        CALL_VK(vkCreateImageView(device->GetHandle(), &imageViewInfo, nullptr, &m_handler));
    }

    CtVKImageView::~CtVKImageView() {
        VK_D(ImageView, m_device->GetHandle(), m_handler);
    }
}
