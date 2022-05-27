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
#ifndef PATH_MONITOR_H
#define PATH_MONITOR_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"

#include <mutex>

using namespace ceammc;

class PathMonitor : public NotifiedObject {
    SymbolProperty* path_;
    std::string path_info_;
    std::mutex mtx_;

public:
    PathMonitor(const PdArgs& args);
    ~PathMonitor();

    bool notify(NotifyEventType code) override;
    void setPath(const char* path);
};

void setup_path_monitor();

#endif // PATH_MONITOR_H
