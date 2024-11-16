//
// Created by JekYUlll on 2024/9/25.
//

#ifndef CTVKGRAPHICCONTEXT_H
#define CTVKGRAPHICCONTEXT_H

#include "CtGraphicContext.h"
#include "CtVKCommon.h"

namespace cte {

    struct QueueFamilyInfo {
        int32_t queueFamilyIndex = -1;
        uint32_t queueCount = 0;
    };

    class CtVKGraphicContext final : public CtGraphicContext {

    public:
        explicit CtVKGraphicContext(CtWindow* window);
        ~CtVKGraphicContext() override;

    private:
        static void PrintPhysicalDeviceInfo(VkPhysicalDeviceProperties& properties);
        static uint32_t GetPhysicalDeviceScore(const VkPhysicalDeviceProperties& properties);

        void CreateInstance();
        void CreateSurface(CtWindow* window);
        void SelectPhysicalDevice();

    public:
        [[nodiscard]] inline bool IsSameGraphicPresentQueueFamily() const {return this->m_graphicQueueFamily.queueFamilyIndex == this->m_presentQueueFamily.queueFamilyIndex;}
        [[nodiscard]] inline VkInstance GetInstance() const { return this->m_instance; }
        [[nodiscard]] inline VkSurfaceKHR GetSurface() const { return this->m_surface; }
        [[nodiscard]] inline VkPhysicalDevice GetPhysicalDevice() const { return this->m_physicalDevice; }
        [[nodiscard]] inline VkPhysicalDeviceMemoryProperties GetMemoryProperties() const { return this->m_memoryProperties; }
        [[nodiscard]] inline const QueueFamilyInfo& GetGraphicQueueFamilyInfo() const { return this->m_graphicQueueFamily; }
        [[nodiscard]] inline const QueueFamilyInfo& GetPresentQueueFamilyInfo() const { return this->m_presentQueueFamily; }

    private:
        bool bShouldValidate = true;

        VkInstance m_instance;
        VkSurfaceKHR m_surface;

        VkPhysicalDevice m_physicalDevice;
        QueueFamilyInfo m_graphicQueueFamily;
        QueueFamilyInfo m_presentQueueFamily;
        VkPhysicalDeviceMemoryProperties m_memoryProperties;
    };


}

#endif //CTVKGRAPHICCONTEXT_H
