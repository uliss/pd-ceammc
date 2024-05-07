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
#include "hw_cpu_temp.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

HwCpuTemp::HwCpuTemp(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
    , cpu_temp_(0, ceammc_hw_cputemp_free)
{
    createOutlet();

    cpu_temp_.reset(ceammc_hw_cputemp_create(
        this,
        [](void* user, const char* label, float value) {
            auto this_ = static_cast<HwCpuTemp*>(user);
            if (this_) {
                AtomArray<2> data { gensym(label), value };
                this_->listTo(0, data.view());
            }
        },
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }));
}

void HwCpuTemp::onBang()
{
    if (cpu_temp_)
        ceammc_hw_cputemp_get(cpu_temp_.get());
}

bool HwCpuTemp::notify(int)
{
    if (cpu_temp_)
        return ceammc_hw_cputemp_process(cpu_temp_.get());
    else
        return false;
}

void setup_hw_cpu_temp()
{
    ObjectFactory<HwCpuTemp> obj("hw.cpu_temp");
}
