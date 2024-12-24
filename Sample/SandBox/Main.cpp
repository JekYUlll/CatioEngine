//
// Created by JekYUlll on 2024/9/23.
//
#include <iostream>
#include "CtLog.h"
#include "CtWindow.h"
#include "CtGraphicContext.h"
#include "Graphic/CtVKGraphicContext.h"
#include "Graphic/CtVKDevice.h"
#include "Graphic/CtVKFrameBuffer.h"
#include "Graphic/CtVKSwapchain.h"
#include "Graphic/CtVKRenderPass.h"
#include "Graphic/CtVKPipeline.h"
#include "CtFileUtil.h"

int main(int argc, char const* argv[]) {

  std::cout << "Hello Catio Engine." << std::endl;

  cte::CtLog::Init();
  LOG_T("Log test: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
  LOG_D("Log test: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
  LOG_I("Log test: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
  LOG_W("Log test: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
  LOG_E("Log test: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);

    std::unique_ptr<cte::CtWindow> window = cte::CtWindow::Create(800, 600, "SandBox");
    std::unique_ptr<cte::CtGraphicContext> graphicContext = cte::CtGraphicContext::Create(window.get());
    auto vkContext = dynamic_cast<cte::CtVKGraphicContext*>(graphicContext.get());
    std::shared_ptr<cte::CtVKDevice> device = std::make_shared<cte::CtVKDevice>(vkContext, 1, 1);
    std::shared_ptr<cte::CtVKSwapchain> swapchain = std::make_shared<cte::CtVKSwapchain>(vkContext, device.get());
    swapchain->ReCreate();
    std::shared_ptr<cte::CtVKRenderPass> renderPass = std::make_shared<cte::CtVKRenderPass>(device.get());

    std::vector<VkImage> swapchainImages = swapchain->GetImages();
    std::vector<std::shared_ptr<cte::CtVKFrameBuffer>> framebuffers;
    for(const auto& image : swapchainImages) {
        std::vector<VkImage> images = { image };
        framebuffers.push_back(std::make_shared<cte::CtVKFrameBuffer>(device.get(), renderPass.get(), images, swapchain->GetWidth(), swapchain->GetHeight()));
    }

    std::shared_ptr<cte::CtVKPipelineLayout> pipelineLayout = std::make_shared<cte::CtVKPipelineLayout>(device.get(),
        CT_RES_SHADER_DIR"00_hello_triangle.vert", CT_RES_SHADER_DIR"00_hello_triangle.frag");
    std::shared_ptr<cte::CtVKPipeline> pipeline = std::make_shared<cte::CtVKPipeline>(device.get(),renderPass.get(), pipelineLayout.get());
    pipeline->SetInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_LINE_LIST)->EnableDepthTest();
    pipeline->SetDynamicState({ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR });
    pipeline->Create();

  while (!window->ShouldClose()) {
      window->PollEvents();
      window->SwapBuffers();
  }

  return EXIT_SUCCESS;
}
