//
// Created by JekYUlll on 2024/11/27.
//

#ifndef CTVKIMAGEVIEW_H
#define CTVKIMAGEVIEW_H

#include "Graphic/CtVKCommon.h"

namespace cte {
    class CtVKDevice;
    class CtVKImageView {
    public:
        CtVKImageView(CtVKDevice* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        ~CtVKImageView();

        [[nodiscard]] VkImageView GetHandler() const { return this->m_handler; }

    private:
        VkImageView m_handler = VK_NULL_HANDLE;
        CtVKDevice* m_device;
    };


}

#endif //CTVKIMAGEVIEW_H
