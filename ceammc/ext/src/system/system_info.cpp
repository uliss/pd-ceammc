/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "system_info.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM(temp)

SystemInfo::SystemInfo(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
    , sysinfo_(nullptr, ceammc_sysinfo_free)
{
    createOutlet();

    sysinfo_.reset(ceammc_sysinfo_create(
        { //
            this,
            [](void* user, const char* label, float value) {
                auto this_ = static_cast<SystemInfo*>(user);
                if (this_) {
                    AtomArray<2> data { value, gensym(label) };
                    this_->anyTo(0, sym_temp(), data.view());
                }
            } },
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }));
}

bool SystemInfo::notify(int)
{
    if (sysinfo_)
        return ceammc_sysinfo_process(sysinfo_.get());
    else
        return false;
}

void SystemInfo::m_temp(t_symbol* s, const AtomListView& lv)
{
    if (sysinfo_)
        ceammc_sysinfo_get_temperature(sysinfo_.get());
}

void setup_system_info()
{
    ObjectFactory<SystemInfo> obj("system.info");
    obj.addMethod("temp", &SystemInfo::m_temp);
}
