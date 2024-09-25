//
// Created by JekYUlll on 2024/9/25.
//

#ifndef CTGRAPHICCONTEXT_H
#define CTGRAPHICCONTEXT_H

#include "CtWindow.h"
#include <memory>


namespace cte {

    class CtWindow;
    class CtGraphicContext {
    public:
        CtGraphicContext(const CtGraphicContext&) = delete;
        CtGraphicContext& operator=(const CtGraphicContext&) = delete;
        virtual ~CtGraphicContext() = default;

        static std::unique_ptr<CtGraphicContext> Create(CtWindow *window);

    protected:
        CtGraphicContext();

    };

}

#endif //CTGRAPHICCONTEXT_H
