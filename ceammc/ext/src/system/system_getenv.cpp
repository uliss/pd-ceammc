/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "system_getenv.h"
#include "ceammc_factory.h"

#include <cstdlib>
#include <cstring>

SystemGetEnv::SystemGetEnv(const PdArgs& a)
    : BaseObject(a)
    , var_(nullptr)
{
    createOutlet();

    var_ = new SymbolProperty("@name", &s_);
    var_->setArgIndex(0);
    addProperty(var_);
}

void SystemGetEnv::onBang()
{
    if (var_->value() == &s_) {
        OBJ_ERR << "no variable name given";
        return;
    }

    auto v = getenv(var_->value()->s_name);

    if (v != NULL && strlen(v) > 0)
        symbolTo(0, gensym(v));
    else
        listTo(0, AtomList());
}

void SystemGetEnv::onSymbol(t_symbol* s)
{
    if (var_->setValue(s))
        onBang();
}

void setup_system_getenv()
{
    ObjectFactory<SystemGetEnv> obj("system.getenv");
}
