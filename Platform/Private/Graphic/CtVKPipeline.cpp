//
// Created by JekYUlll on 2024/11/27.
//
#include "Graphic/CtVKPipeline.h"

namespace cte {
    CtVKPipelineLayout::CtVKPipelineLayout(CtVKDevice *device, const std::string &vertexShaderFile, const std::string &fragmentShaderFile, const ShaderLayout &shaderLayout) {
        // compile shaders


        // pipeline layout

    }



    CtVKPipelineLayout::~CtVKPipelineLayout() {
    }

    VkResult CtVKPipelineLayout::CreateShaderModule(const std::string &filePath, VkShaderModule *out_shaderModule) {


    }
}
