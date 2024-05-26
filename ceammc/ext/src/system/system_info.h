/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "system_rust.hpp"

#include <memory>
using namespace ceammc;

class SystemInfo : public DispatchedObject<BaseObject> {
    std::unique_ptr<ceammc_system_info, void (*)(ceammc_system_info*)> sysinfo_;

public:
    SystemInfo(const PdArgs& args);

    bool notify(int) final;
    void m_temp(t_symbol* s, const AtomListView& lv);
    void m_cpu(t_symbol* s, const AtomListView& lv);
};

void setup_system_info();

#endif // SYSTEM_INFO_H
