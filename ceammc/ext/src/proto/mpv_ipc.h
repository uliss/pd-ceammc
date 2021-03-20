/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef MPV_IPC_H
#define MPV_IPC_H

#include "readerwriterqueue.h"

#include <atomic>
#include <future>
#include <string>

namespace ceammc {
namespace mpv {

    using StringQueue = moodycamel::ReaderWriterQueue<std::string, 16>;
    using MpvIpcFuture = std::future<void>;

    void mpv_ipc(const std::string& ipc_name, StringQueue& queue_in, StringQueue& queue_out, std::atomic_bool& quit);

}
}

#endif // MPV_IPC_H
