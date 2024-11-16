//
// Created by JekYUlll on 2024/9/23.
//
#include <iostream>
#include "CtLog.h"
#include "CtWindow.h"
#include "CtGraphicContext.h"
#include "Graphic/CtVKGraphicContext.h"
#include "Graphic/CtVKDevice.h"
#include "Graphic/CtVKSwapChain.h"

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

  while (!window->ShouldClose()) {
      window->PollEvents();
      window->SwapBuffers();
  }

  return EXIT_SUCCESS;
}
