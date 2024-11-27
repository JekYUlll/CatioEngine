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
                    // 此处有冲突 m_device->GetSettings().surfaceFormat 提供。调试信息显示其值为 VK_FORMAT_B8G8R8A8_UNORM：不一致
                    // .format = VK_FORMAT_R8G8B8A8_SRGB,
                    .format = VK_FORMAT_B8G8R8A8_UNORM,
                    .samples = VK_SAMPLE_COUNT_1_BIT,
                    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                };
                m_attachments.push_back(defaultColorAttachment);
            }
            RenderSubPass defaultSubPass = {
                .colorAttachment = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }
            };
            this->m_subPasses.push_back(defaultSubPass);
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
            subpassDescriptions[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDescriptions[i].inputAttachmentCount = subpass.inputAttachment.ref >= 0 ? ARRAY_SIZE(inputAttachmentRef) : 0;
            subpassDescriptions[i].pInputAttachments = subpass.inputAttachment.ref >= 0 ? inputAttachmentRef : nullptr;
            subpassDescriptions[i].colorAttachmentCount = subpass.colorAttachment.ref >= 0 ? ARRAY_SIZE(colorAttachmentRef) : 0;
            subpassDescriptions[i].pColorAttachments = subpass.colorAttachment.ref >= 0 ? colorAttachmentRef : nullptr;
            subpassDescriptions[i].pResolveAttachments = subpass.resolveAttachment.ref >= 0 ? resolveAttachmentRef : nullptr;
            subpassDescriptions[i].pDepthStencilAttachment = subpass.depthStencilAttachment.ref >= 0 ? depthStencilAttachmentRef : nullptr;
            subpassDescriptions[i].preserveAttachmentCount = 0;
            subpassDescriptions[i].pPreserveAttachments = nullptr;
        }
        std::vector<VkSubpassDependency> subpassDependencies(m_subPasses.size() - 1);
        if(m_subPasses.size() > 1) {
            for(int i = 0; i < subpassDependencies.size(); i++) {
                subpassDependencies[i].srcSubpass = i;
                subpassDependencies[i].dstSubpass = i + 1;
                subpassDependencies[i].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependencies[i].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                subpassDependencies[i].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependencies[i].dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
                subpassDependencies[i].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            }
        }

        // 3. createinfo
        VkRenderPassCreateInfo renderPassInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .attachmentCount = static_cast<uint32_t>(m_subPasses.size()),
            .pAttachments = m_attachments.data(),
            .subpassCount = static_cast<uint32_t>(m_subPasses.size()),
            .pSubpasses = subpassDescriptions.data(),
            .dependencyCount = static_cast<uint32_t>(subpassDependencies.size()),
            .pDependencies = nullptr
        };

        CALL_VK(vkCreateRenderPass(m_device->GetHandle(), &renderPassInfo, nullptr, &m_handle));
        LOG_T("RenderPass {0} : {1}, attachment count: {2}, subpass count: {3}", __FUNCTION__, reinterpret_cast<void *>(m_handle), m_attachments.size(), m_subPasses.size());
        // 在 RenderPass 创建时打印格式信息
        LOG_T("RenderPass format: {0}", m_attachments[0].format);
    }

    CtVKRenderPass::~CtVKRenderPass() {
        VK_D(RenderPass, m_device->GetHandle(), m_handle);
    }
}


