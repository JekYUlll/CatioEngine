//
// Created by JekYUlll on 2024/9/29.
//

#ifndef CTVKRENDERPASS_H
#define CTVKRENDERPASS_H
#include <vector>
#include "Graphic/CtVKCommon.h"

namespace cte {

    class CtVKDevice;

    // 附件
    struct Attachment {
        int32_t ref = -1; // 索引
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    };

    // RenderPass子流程
    struct RenderSubPass {
        Attachment inputAttachment;
        Attachment colorAttachment;
        Attachment depthStencilAttachment;
        Attachment resolveAttachment;
    };

    class CtVKRenderPass {
    public:
        explicit CtVKRenderPass(CtVKDevice* device, const std::vector<VkAttachmentDescription>& attachments = {},
                                            const std::vector<RenderSubPass>& subPasses = {});
        ~CtVKRenderPass();

        [[nodiscard]] VkRenderPass GetHandle() const { return this->m_handle; }

    private:
        VkRenderPass m_handle = VK_NULL_HANDLE;
        CtVKDevice* m_device;

        std::vector<VkAttachmentDescription> m_attachments;
        std::vector<RenderSubPass> m_subPasses;
    };


}

#endif //CTVKRENDERPASS_H
