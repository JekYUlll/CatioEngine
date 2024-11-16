//
// Created by JekYUlll on 2024/9/28.
//

#ifndef CTVKQUEUE_H
#define CTVKQUEUE_H

#include "CtVKCommon.h"

namespace cte {

    class CtVKQueue {
    public:
        CtVKQueue(uint32_t familyIndex, uint32_t index, VkQueue queue, bool canPresent);
        ~CtVKQueue() = default;

        void WaitIdle() const;

    private:
        uint32_t m_familyIndex;
        uint32_t m_index;
        VkQueue m_queue;
        bool canPresent;
    };

}

#endif //CTVKQUEUE_H
