//
// Created by JekYUlll on 2024/11/27.
//

#ifndef CTVKPIPELINE_H
#define CTVKPIPELINE_H

#include "Graphic/CtVKCommon.h"

namespace cte {

    class CtVKDevice;
    class CtVKRenderPass;

    struct ShaderLayout {
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        std::vector<VkPushConstantRange> pushConstants;
    };

    struct PipelineVertexInputState{
        std::vector<VkVertexInputBindingDescription> vertexBindings;
        std::vector<VkVertexInputAttributeDescription> vertexAttributes;
    };

    struct PipelineInputAssemblyState {
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkBool32 primitiveRestartEnable = VK_FALSE;
    };

    struct PipelineRasterizationState{
        VkBool32 depthClampEnable = VK_FALSE;
        VkBool32 rasterizerDiscardEnable = VK_FALSE;
        VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
        VkCullModeFlags cullMode = VK_CULL_MODE_NONE;
        VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        VkBool32 depthBiasEnable = VK_FALSE;
        float depthBiasConstantFactor = 0;
        float depthBiasClamp = 0;
        float depthBiasSlopeFactor = 0;
        float lineWidth = 1.f;
    };

    struct PipelineMultisampleState{
        VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        VkBool32 sampleShadingEnable = VK_FALSE;
        float minSampleShading = 0.2f;
    };

    struct PipelineDepthStencilState{
        VkBool32 depthTestEnable = VK_FALSE;
        VkBool32 depthWriteEnable = VK_FALSE;
        VkCompareOp depthCompareOp = VK_COMPARE_OP_NEVER;
        VkBool32 depthBoundsTestEnable = VK_FALSE;
        VkBool32 stencilTestEnable = VK_FALSE;
    };

    struct PipelineDynamicState{
        std::vector<VkDynamicState> dynamicStates;
    };

    struct PipelineConfig{
        PipelineVertexInputState vertexInputState;
        PipelineInputAssemblyState inputAssemblyState;
        PipelineRasterizationState rasterizationState;
        PipelineMultisampleState multisampleState;
        PipelineDepthStencilState depthStencilState;
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState{
                .blendEnable = VK_FALSE,
                .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                .colorBlendOp = VK_BLEND_OP_ADD,
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                .alphaBlendOp = VK_BLEND_OP_ADD,
                .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                                  | VK_COLOR_COMPONENT_G_BIT
                                  | VK_COLOR_COMPONENT_B_BIT
                                  | VK_COLOR_COMPONENT_A_BIT
        };
        PipelineDynamicState dynamicState;
    };

    class CtVKPipelineLayout {
    public:
        CtVKPipelineLayout(CtVKDevice* device, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const ShaderLayout& shaderLayout = {});
        ~CtVKPipelineLayout();

        [[nodiscard]] VkPipelineLayout GetHandle() const { return this->m_handle; };
        [[nodiscard]] VkShaderModule GetVertexShaderModule() const { return this->m_vertexShaderModule; };
        [[nodiscard]] VkShaderModule GetFragmentShaderModule() const { return this->m_fragmentShaderModule; };

    private:
        VkResult CreateShaderModule(const std::string& filePath, VkShaderModule* out_shaderModule) const;

        VkPipelineLayout m_handle = VK_NULL_HANDLE;

        CtVKDevice* m_device{};

        VkShaderModule m_vertexShaderModule = VK_NULL_HANDLE;
        VkShaderModule m_fragmentShaderModule = VK_NULL_HANDLE;
    };

    class CtVKPipeline {
    public:
        CtVKPipeline(CtVKDevice* device, CtVKRenderPass* renderPass, CtVKPipelineLayout* pipelineLayout);
        ~CtVKPipeline();

        void Create();
        void Bind(VkCommandBuffer cmdBuffer);

        CtVKPipeline *SetVertexInputState(const std::vector<VkVertexInputBindingDescription> &vertexBindings, const std::vector<VkVertexInputAttributeDescription> &vertexAttrs);
        CtVKPipeline *SetInputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable = VK_FALSE);
        CtVKPipeline *SetRasterizationState(const PipelineRasterizationState &rasterizationState);
        CtVKPipeline *SetMultisampleState(VkSampleCountFlagBits samples, VkBool32 sampleShadingEnable, float minSampleShading = 0.f);
        CtVKPipeline *SetDepthStencilState(const PipelineDepthStencilState &depthStencilState);
        CtVKPipeline *SetColorBlendAttachmentState(VkBool32 blendEnable,
                                                   VkBlendFactor srcColorBlendFactor = VK_BLEND_FACTOR_ONE, VkBlendFactor dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, VkBlendOp colorBlendOp = VK_BLEND_OP_ADD,
                                                   VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, VkBlendOp alphaBlendOp = VK_BLEND_OP_ADD);
        CtVKPipeline *SetDynamicState(const std::vector<VkDynamicState> &dynamicStates);
        CtVKPipeline *EnableAlphaBlend();
        CtVKPipeline *EnableDepthTest();

        [[nodiscard]] VkPipeline GetHandle() const { return this->m_handle; };

    private:
        VkPipeline m_handle = VK_NULL_HANDLE;
        CtVKDevice* m_device{};
        CtVKRenderPass *m_renderPass;
        CtVKPipelineLayout *m_pipelineLayout;

        PipelineConfig m_pipelineConfig;
    };

}

#endif //CTVKPIPELINE_H
