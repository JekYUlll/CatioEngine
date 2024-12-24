//
// Created by JekYUlll on 2024/11/27.
//
#include "Graphic/CtVKPipeline.h"

#include <Graphic/CtVKRenderPass.h>

#include "CtFileUtil.h"
#include "Graphic/CtVKDevice.h"

namespace cte {

    ///// pipeline layout

    CtVKPipelineLayout::CtVKPipelineLayout(CtVKDevice *device, const std::string &vertexShaderFile, const std::string &fragmentShaderFile, const ShaderLayout &shaderLayout)
        : m_device(device) {
        // compile shaders
        CALL_VK(CreateShaderModule(vertexShaderFile + ".spv", &m_vertexShaderModule));
        CALL_VK(CreateShaderModule(fragmentShaderFile + ".spv", &m_fragmentShaderModule));
        // pipeline layout
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = static_cast<uint32_t>(shaderLayout.descriptorSetLayouts.size()),
            .pSetLayouts = shaderLayout.descriptorSetLayouts.data(),
            .pushConstantRangeCount = static_cast<uint32_t>(shaderLayout.pushConstants.size()),
            .pPushConstantRanges = shaderLayout.pushConstants.data(),
        };
        CALL_VK(vkCreatePipelineLayout(m_device->GetHandle(), &pipelineLayoutInfo,nullptr, &m_handle));
    }

    CtVKPipelineLayout::~CtVKPipelineLayout() {
        VK_D(ShaderModule, m_device->GetHandle(), m_vertexShaderModule);
        VK_D(ShaderModule, m_device->GetHandle(), m_fragmentShaderModule);
        VK_D(PipelineLayout, m_device->GetHandle(), m_handle);
    }

    VkResult CtVKPipelineLayout::CreateShaderModule(const std::string &filePath, VkShaderModule *out_shaderModule) const {
        std::vector<char> content = ReadCharArrayFromFile(filePath);
        VkShaderModuleCreateInfo shaderModuleInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .codeSize = static_cast<uint32_t>(content.size()),
            .pCode = reinterpret_cast<const uint32_t *>(content.data())
        };
        return vkCreateShaderModule(m_device->GetHandle(), &shaderModuleInfo, nullptr, out_shaderModule);
    }

    ///// pipeline

    CtVKPipeline::CtVKPipeline(CtVKDevice *device, CtVKRenderPass *renderPass, CtVKPipelineLayout *pipelineLayout)
        : m_device(device) {
        VkGraphicsPipelineCreateInfo graphicsPipelineInfo = {

        };
        CALL_VK(vkCreateGraphicsPipelines(m_device->GetHandle(), m_device->GetPipelineCache(), 1, &graphicsPipelineInfo, nullptr, &m_handle));
    }

    CtVKPipeline::~CtVKPipeline() {
        VK_D(Pipeline, m_device->GetHandle(), m_handle);
    }

    void CtVKPipeline::Create() {
        VkPipelineShaderStageCreateInfo shaderStageInfo[] = {
                {
                        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .pNext = nullptr,
                        .flags = 0,
                        .stage = VK_SHADER_STAGE_VERTEX_BIT,
                        .module = m_pipelineLayout->GetVertexShaderModule(),
                        .pName = "main",
                        .pSpecializationInfo = nullptr
                },
                {
                        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .pNext = nullptr,
                        .flags = 0,
                        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                        .module = m_pipelineLayout->GetFragmentShaderModule(),
                        .pName = "main",
                        .pSpecializationInfo = nullptr
                }
        };

        VkPipelineVertexInputStateCreateInfo vertexInputStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .vertexBindingDescriptionCount = static_cast<uint32_t>(m_pipelineConfig.vertexInputState.vertexBindings.size()),
                .pVertexBindingDescriptions = m_pipelineConfig.vertexInputState.vertexBindings.data(),
                .vertexAttributeDescriptionCount = static_cast<uint32_t>(m_pipelineConfig.vertexInputState.vertexAttributes.size()),
                .pVertexAttributeDescriptions = m_pipelineConfig.vertexInputState.vertexAttributes.data()
        };

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .topology = m_pipelineConfig.inputAssemblyState.topology,
                .primitiveRestartEnable = m_pipelineConfig.inputAssemblyState.primitiveRestartEnable
        };

        VkViewport defaultViewport = {
                .x = 0,
                .y = 0,
                .width = 100,
                .height = 100,
                .minDepth = 0,
                .maxDepth = 1
        };
        VkRect2D defaultScissor = {
                .offset = { 0, 0 },
                .extent = { 100, 100 }
        };
        VkPipelineViewportStateCreateInfo viewportStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .viewportCount = 1,
                .pViewports = &defaultViewport,
                .scissorCount = 1,
                .pScissors = &defaultScissor
        };

        VkPipelineRasterizationStateCreateInfo rasterizationStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .depthClampEnable = m_pipelineConfig.rasterizationState.depthClampEnable,
                .rasterizerDiscardEnable = m_pipelineConfig.rasterizationState.rasterizerDiscardEnable,
                .polygonMode = m_pipelineConfig.rasterizationState.polygonMode,
                .cullMode = m_pipelineConfig.rasterizationState.cullMode,
                .frontFace = m_pipelineConfig.rasterizationState.frontFace,
                .depthBiasEnable = m_pipelineConfig.rasterizationState.depthBiasEnable,
                .depthBiasConstantFactor = m_pipelineConfig.rasterizationState.depthBiasConstantFactor,
                .depthBiasClamp = m_pipelineConfig.rasterizationState.depthBiasClamp,
                .depthBiasSlopeFactor = m_pipelineConfig.rasterizationState.depthBiasSlopeFactor,
                .lineWidth = m_pipelineConfig.rasterizationState.lineWidth
        };

        VkPipelineMultisampleStateCreateInfo multisampleStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .rasterizationSamples = m_pipelineConfig.multisampleState.rasterizationSamples,
                .sampleShadingEnable = m_pipelineConfig.multisampleState.sampleShadingEnable,
                .minSampleShading = m_pipelineConfig.multisampleState.minSampleShading,
                .pSampleMask = nullptr,
                .alphaToCoverageEnable = VK_FALSE,
                .alphaToOneEnable = VK_FALSE
        };

        VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .depthTestEnable = m_pipelineConfig.depthStencilState.depthTestEnable,
                .depthWriteEnable = m_pipelineConfig.depthStencilState.depthWriteEnable,
                .depthCompareOp = m_pipelineConfig.depthStencilState.depthCompareOp,
                .depthBoundsTestEnable = m_pipelineConfig.depthStencilState.depthBoundsTestEnable,
                .stencilTestEnable = m_pipelineConfig.depthStencilState.stencilTestEnable,
                .front = {},
                .back = {},
                .minDepthBounds = 0.0f,
                .maxDepthBounds = 0.0f
        };

        VkPipelineColorBlendStateCreateInfo colorBlendStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_CLEAR,
                .attachmentCount = 1,
                .pAttachments = &m_pipelineConfig.colorBlendAttachmentState,
        };
        colorBlendStateInfo.blendConstants[0] = colorBlendStateInfo.blendConstants[1] = colorBlendStateInfo.blendConstants[2] = colorBlendStateInfo.blendConstants[3] = 0;

        VkPipelineDynamicStateCreateInfo dynamicStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .dynamicStateCount = static_cast<uint32_t>(m_pipelineConfig.dynamicState.dynamicStates.size()),
                .pDynamicStates = m_pipelineConfig.dynamicState.dynamicStates.data()
        };
        VkGraphicsPipelineCreateInfo pipelineInfo = {
                .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .stageCount = ARRAY_SIZE(shaderStageInfo),
                .pStages = shaderStageInfo,
                .pVertexInputState = &vertexInputStateInfo,
                .pInputAssemblyState = &inputAssemblyStateInfo,
                .pTessellationState = nullptr,
                .pViewportState = &viewportStateInfo,
                .pRasterizationState = &rasterizationStateInfo,
                .pMultisampleState = &multisampleStateInfo,
                .pDepthStencilState = &depthStencilStateInfo,
                .pColorBlendState = &colorBlendStateInfo,
                .pDynamicState = &dynamicStateInfo,
                .layout = m_pipelineLayout->GetHandle(),
                .renderPass = m_renderPass->GetHandle(),
                .subpass = 0,
                .basePipelineHandle = VK_NULL_HANDLE,
                .basePipelineIndex = 0
        };
        CALL_VK(vkCreateGraphicsPipelines(m_device->GetHandle(), m_device->GetPipelineCache(), 1, &pipelineInfo, nullptr, &m_handle));
        LOG_T("Create pipeline : {0}", (void*)m_handle);
    }

    CtVKPipeline *CtVKPipeline::SetVertexInputState(const std::vector<VkVertexInputBindingDescription> &vertexBindings,
                                                    const std::vector<VkVertexInputAttributeDescription> &vertexAttrs) {
        m_pipelineConfig.vertexInputState.vertexBindings = vertexBindings;
        m_pipelineConfig.vertexInputState.vertexAttributes = vertexAttrs;
        return this;
    }

    CtVKPipeline *CtVKPipeline::SetInputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable) {
        m_pipelineConfig.inputAssemblyState.topology = topology;
        m_pipelineConfig.inputAssemblyState.primitiveRestartEnable = primitiveRestartEnable;
        return this;
    }

    CtVKPipeline *CtVKPipeline::SetRasterizationState(const PipelineRasterizationState &rasterizationState) {
        m_pipelineConfig.rasterizationState.depthClampEnable = rasterizationState.depthClampEnable;
        m_pipelineConfig.rasterizationState.rasterizerDiscardEnable = rasterizationState.rasterizerDiscardEnable;
        m_pipelineConfig.rasterizationState.polygonMode = rasterizationState.polygonMode;
        m_pipelineConfig.rasterizationState.cullMode = rasterizationState.cullMode;
        m_pipelineConfig.rasterizationState.frontFace = rasterizationState.frontFace;
        m_pipelineConfig.rasterizationState.depthBiasEnable = rasterizationState.depthBiasEnable;
        m_pipelineConfig.rasterizationState.depthBiasConstantFactor = rasterizationState.depthBiasConstantFactor;
        m_pipelineConfig.rasterizationState.depthBiasClamp = rasterizationState.depthBiasClamp;
        m_pipelineConfig.rasterizationState.depthBiasSlopeFactor = rasterizationState.depthBiasSlopeFactor;
        m_pipelineConfig.rasterizationState.lineWidth = rasterizationState.lineWidth;
        return this;
    }

    CtVKPipeline *CtVKPipeline::SetMultisampleState(VkSampleCountFlagBits samples, VkBool32 sampleShadingEnable, float minSampleShading) {
        m_pipelineConfig.multisampleState.rasterizationSamples = samples;
        m_pipelineConfig.multisampleState.sampleShadingEnable = sampleShadingEnable;
        m_pipelineConfig.multisampleState.minSampleShading = minSampleShading;
        return this;
    }

    CtVKPipeline *CtVKPipeline::SetDepthStencilState(const PipelineDepthStencilState &depthStencilState) {
        m_pipelineConfig.depthStencilState.depthTestEnable = depthStencilState.depthTestEnable;
        m_pipelineConfig.depthStencilState.depthWriteEnable = depthStencilState.depthWriteEnable;
        m_pipelineConfig.depthStencilState.depthCompareOp = depthStencilState.depthCompareOp;
        m_pipelineConfig.depthStencilState.depthBoundsTestEnable = depthStencilState.depthBoundsTestEnable;
        m_pipelineConfig.depthStencilState.stencilTestEnable = depthStencilState.stencilTestEnable;
        return this;
    }

    CtVKPipeline *CtVKPipeline::SetColorBlendAttachmentState(VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor,
                                                             VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp,
                                                             VkBlendFactor srcAlphaBlendFactor,
                                                             VkBlendFactor dstAlphaBlendFactor,
                                                             VkBlendOp alphaBlendOp) {
        m_pipelineConfig.colorBlendAttachmentState.blendEnable = blendEnable;
        m_pipelineConfig.colorBlendAttachmentState.srcColorBlendFactor = srcColorBlendFactor;
        m_pipelineConfig.colorBlendAttachmentState.dstColorBlendFactor = dstColorBlendFactor;
        m_pipelineConfig.colorBlendAttachmentState.srcAlphaBlendFactor = srcAlphaBlendFactor;
        m_pipelineConfig.colorBlendAttachmentState.dstAlphaBlendFactor = dstAlphaBlendFactor;
        m_pipelineConfig.colorBlendAttachmentState.alphaBlendOp = alphaBlendOp;
        return this;
    }

    CtVKPipeline *CtVKPipeline::SetDynamicState(const std::vector<VkDynamicState> &dynamicStates) {
        m_pipelineConfig.dynamicState.dynamicStates = dynamicStates;
        return this;
    }

    CtVKPipeline *CtVKPipeline::EnableAlphaBlend() {
        m_pipelineConfig.colorBlendAttachmentState = {
                .blendEnable = VK_TRUE,
                .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
                .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                .colorBlendOp = VK_BLEND_OP_ADD,
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                .alphaBlendOp = VK_BLEND_OP_ADD
        };
        return this;
    }

    CtVKPipeline *CtVKPipeline::EnableDepthTest() {
        m_pipelineConfig.depthStencilState.depthTestEnable = VK_TRUE;
        m_pipelineConfig.depthStencilState.depthWriteEnable = VK_TRUE;
        m_pipelineConfig.depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
        return this;
    }
}
