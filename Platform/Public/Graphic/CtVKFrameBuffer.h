//
// Created by JekYUlll on 2024/11/27.
//

#ifndef CTVKFRAMEBUFFER_H
#define CTVKFRAMEBUFFER_H

#include "Graphic/CtVKCommon.h"

namespace cte {
    class CtVKRenderPass;
    class CtVKDevice;
    class CtVKImageView;

    class CtVKFrameBuffer {
    public:
        CtVKFrameBuffer(CtVKDevice* device, CtVKRenderPass* renderPass, const std::vector<VkImage>& images, uint32_t width, uint32_t height);
        ~CtVKFrameBuffer();

        bool ReCreate(const std::vector<VkImage>& images, uint32_t width, uint32_t height);

    private:
        VkFramebuffer m_handler{};
        CtVKDevice* m_device;
        CtVKRenderPass* m_renderPass;
        uint32_t m_width;
        uint32_t m_height;
        std::vector<std::shared_ptr<CtVKImageView>> m_imageViews;
    };
}

#endif //CTVKFRAMEBUFFER_H
