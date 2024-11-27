//
// Created by JekYUlll on 2024/11/27.
//

#ifndef CTVKPIPELINE_H
#define CTVKPIPELINE_H

#include "Graphic/CtVKCommon.h"

namespace cte {

    class CtVKDevice;

    struct ShaderLayout {
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        std::vector<VkPushConstantRange> pushConstants;
    };

    class CtVKPipelineLayout {
    public:
        CtVKPipelineLayout(CtVKDevice* device, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const ShaderLayout& shaderLayout = {});
        ~CtVKPipelineLayout();

        [[nodiscard]] VkPipelineLayout GetHandle() const { return this->m_handle; };
        [[nodiscard]] VkShaderModule GetVertexShaderModule() const { return this->m_vertexShaderModule; };
        [[nodiscard]] VkShaderModule GetFragmentShaderModule() const { return this->m_fragmentShaderModule; };

    private:
        VkResult CreateShaderModule(const std::string& filePath, VkShaderModule* out_shaderModule);

        VkPipelineLayout m_handle = VK_NULL_HANDLE;

        CtVKDevice* device;

        VkShaderModule m_vertexShaderModule = VK_NULL_HANDLE;
        VkShaderModule m_fragmentShaderModule = VK_NULL_HANDLE;
    };

    class CtVKPipeline {
    public:


    };

}

#endif //CTVKPIPELINE_H
