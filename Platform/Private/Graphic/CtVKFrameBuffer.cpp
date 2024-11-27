//
// Created by JekYUlll on 2024/11/27.
//

#include "Graphic/CtVKFrameBuffer.h"
#include "Graphic/CtVKDevice.h"
#include "Graphic/CtVKRenderPass.h"
#include "Graphic/CtVKImageView.h"

namespace cte {
    CtVKFrameBuffer::CtVKFrameBuffer(CtVKDevice *device, CtVKRenderPass *renderPass, const std::vector<VkImage> &images, uint32_t width, uint32_t height)
        : m_device(device), m_renderPass(renderPass), m_width(width), m_height(height) {
        ReCreate(images, width, height);
    }

    CtVKFrameBuffer::~CtVKFrameBuffer() {
        VK_D(Framebuffer, m_device->GetHandle(), m_handler);
    }

    bool CtVKFrameBuffer::ReCreate(const std::vector<VkImage> &images, uint32_t width, uint32_t height) {
        VK_D(Framebuffer, m_device->GetHandle(), m_handler);

        m_width = width;
        m_height = height;

        m_imageViews.clear();
        std::vector<VkImageView> attachments(images.size());

        for(int i = 0; i < images.size(); i++) {
            m_imageViews.push_back(std::make_shared<CtVKImageView>(m_device, images[i], m_device->GetSettings().surfaceFormat, VK_IMAGE_ASPECT_COLOR_BIT));
            attachments[i] = m_imageViews[i]->GetHandler();
        }

        VkFramebufferCreateInfo frameBufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .renderPass = m_renderPass->GetHandle(),
            .attachmentCount = static_cast<uint32_t>(m_imageViews.size()),
            .pAttachments = attachments.data(),  // ??????
            .width = width,
            .height = height,
            .layers = 1,
        };

        const VkResult result = vkCreateFramebuffer(m_device->GetHandle(), &frameBufferInfo, nullptr, &m_handler);
        LOG_T("FrameBuffer {0}, new: {1}, width: {2}, height: {3}, view count: {4}", __FUNCTION__, reinterpret_cast<void *>(m_handler), m_width, m_height, m_imageViews.size());
        // 在 Framebuffer 创建时打印格式信息
        LOG_T("Framebuffer image format: {0}", m_device->GetSettings().surfaceFormat);
        return result == VK_SUCCESS;
    }
}


