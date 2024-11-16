//
// Created by JekYUlll on 2024/9/28.
//
#include "Graphic/CtVKQueue.h"

namespace cte {

    CtVKQueue::CtVKQueue(uint32_t familyIndex, uint32_t index, VkQueue queue, bool canPresent)
        : m_familyIndex(familyIndex), m_index(index), m_queue(queue), canPresent(canPresent) {
        LOG_T("Create a new queue: {0} - {1} - {2}, present: {3}", this->m_familyIndex, this->m_index, static_cast<void *>(this->m_queue), this->canPresent);
    }

    void CtVKQueue::WaitIdle() const {
        CALL_VK(vkQueueWaitIdle(this->m_queue));
    }

}
