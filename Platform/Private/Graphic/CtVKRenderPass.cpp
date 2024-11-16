//
// Created by JekYUlll on 2024/9/29.
//
#include "Graphic/CtVKRenderPass.h"
#include "Graphic/CtVKDevice.h"

namespace cte {
    CtVKRenderPass::CtVKRenderPass(CtVKDevice *device, const std::vector<VkAttachmentDescription> &attachments, const std::vector<RenderSubPass> &subPasses)
        : m_device(device), m_attachments(attachments), m_subPasses(subPasses) {
        // 1. default subpass and attachment
        if(subPasses.empty()) {
            if(attachments.empty()) {
                const VkAttachmentDescription defaultColorAttachment = {
                    .flags = 0,
                    .format = VK_FORMAT_R8G8B8A8_SRGB,
                    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                };
                m_attachments.push_back(defaultColorAttachment);
            }
            RenderSubPass defaultSubPass = {
                .colorAttachment = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }
            };
        }
        // 2. subpasses
        std::vector<VkSubpassDescription> subpassDescriptions(m_subPasses.size());
        for(size_t i = 0; i < m_subPasses.size(); i++) {
            RenderSubPass subpass = m_subPasses[i];
            if(subpass.inputAttachment.ref >= 0 && subpass.inputAttachment.layout == VK_IMAGE_LAYOUT_UNDEFINED) {
                subpass.inputAttachment.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            }
            if(subpass.colorAttachment.ref >= 0 && subpass.colorAttachment.layout == VK_IMAGE_LAYOUT_UNDEFINED) {
                subpass.colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
            if(subpass.depthStencilAttachment.ref >= 0 && subpass.depthStencilAttachment.layout == VK_IMAGE_LAYOUT_UNDEFINED) {
                subpass.depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            }
            if(subpass.resolveAttachment.ref >= 0 && subpass.resolveAttachment.layout == VK_IMAGE_LAYOUT_UNDEFINED) {
                subpass.resolveAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
            VkAttachmentReference inputAttachmentRef[] = {{ static_cast<uint32_t>(subpass.inputAttachment.ref), subpass.inputAttachment.layout }};
            VkAttachmentReference colorAttachmentRef[] = {{ static_cast<uint32_t>(subpass.colorAttachment.ref), subpass.colorAttachment.layout }};
            VkAttachmentReference depthStencilAttachmentRef[] = {{ static_cast<uint32_t>(subpass.depthStencilAttachment.ref), subpass.depthStencilAttachment.layout }};
            VkAttachmentReference resolveAttachmentRef[] = {{ static_cast<uint32_t>(subpass.resolveAttachment.ref), subpass.resolveAttachment.layout }};
            subpassDescriptions[i].flags = 0;
        }
        // 3. createinfo


    }




    CtVKRenderPass::~CtVKRenderPass() {
    }
}


