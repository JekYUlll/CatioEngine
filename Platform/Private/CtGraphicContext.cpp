//
// Created by JekYUlll on 2024/9/25.
//
#include "CtGraphicContext.h"
#include "Graphic/CtVKGraphicContext.h"

namespace cte {
    std::unique_ptr<CtGraphicContext> CtGraphicContext::Create(CtWindow *window) {
#ifdef CT_ENGINE_GRAPHIC_API_VULKAN
        return std::make_unique<CtVKGraphicContext>(window);
#else

#endif
    }

    CtGraphicContext::CtGraphicContext() {
    }
}
