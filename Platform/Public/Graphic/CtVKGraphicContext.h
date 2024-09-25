//
// Created by JekYUlll on 2024/9/25.
//

#ifndef CTVKGRAPHICCONTEXT_H
#define CTVKGRAPHICCONTEXT_H

#include "CtGraphicContext.h"
#include "CtVKCommon.h"

namespace cte {

    class CtVKGraphicContext : public CtGraphicContext {
    public:
        CtVKGraphicContext(CtWindow* window);
        ~CtVKGraphicContext() override;

    private:
        void CreateInstance();

    private:
        VkInstance m_instance;

    };


}

#endif //CTVKGRAPHICCONTEXT_H
