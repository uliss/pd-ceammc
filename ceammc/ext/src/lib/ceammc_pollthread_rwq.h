/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef CEAMMC_POLLTHREAD_RWQ_H
#define CEAMMC_POLLTHREAD_RWQ_H

#include "ceammc_pollthread_object.h"
#include "readerwriterqueue.h"

namespace ceammc {

template <typename T>
using PollThreadQueue = moodycamel::ReaderWriterQueue<T>;

template <typename In, typename Out>
class PollThreadQueueObject
    : public PollThreadTaskObject<
          PollThreadQueue<In>,
          PollThreadQueue<Out>> {
public:
    PollThreadQueueObject(const PdArgs& args)
        : PollThreadTaskObject<
            PollThreadQueue<In>,
            PollThreadQueue<Out>>(args)
    {
    }

    void processTask(int /*event*/) override
    {
        Out msg;
        while (this->outPipe().try_dequeue(msg))
            processMessage(msg);
    }

    virtual void processMessage(const Out& msg) = 0;
};

}

#endif // CEAMMC_POLLTHREAD_RWQ_H
