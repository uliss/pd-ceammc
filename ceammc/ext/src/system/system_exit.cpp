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
#include "system_exit.h"
#include "ceammc_factory.h"

SystemExit::SystemExit(const PdArgs& args)
    : BaseObject(args)
    , clock_([this]() { exit(rc_->value()); })
    , delay_(nullptr)
    , rc_(nullptr)
    , auto_(nullptr)
{
    delay_ = new FloatProperty("@delay", 0);
    delay_->checkMinEq(0);
    delay_->setUnitsMs();
    delay_->setArgIndex(0);
    addProperty(delay_);

    rc_ = new IntProperty("@rc", 0);
    addProperty(rc_);

    auto_ = new BoolProperty("@auto", false);
    addProperty(auto_);
}

void SystemExit::initDone()
{
    if (auto_->value())
        clock_.delay(delay_->value());
}

void SystemExit::m_exit(t_symbol* s, const AtomListView& lv)
{
    rc_->setList(lv);
    clock_.delay(delay_->value());
}

void SystemExit::exit(int rc)
{
    sys_vgui("exit %d\n", rc);
}

void setup_system_exit()
{
    ObjectFactory<SystemExit> obj("system.exit");
    obj.addMethod("exit", &SystemExit::m_exit);
}
