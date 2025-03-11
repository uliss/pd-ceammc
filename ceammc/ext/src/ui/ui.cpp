/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#include "ui.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "fmt/core.h"
#include "ui_control.tcl.h"
using namespace ceammc;

UIControl::UIControl(const PdArgs& args)
    : BaseObject(args)
{
}

void UIControl::m_open(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FILE:s");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto file = lv.symbolAt(0, &s_)->s_name;
    auto path = findInStdPaths(file);
    if (path.empty()) {
        OBJ_ERR << fmt::format("file not found: '{}'", file);
        return;
    }

    sys_vgui("::ui::open_file {%p} {%s}\n", this, path.c_str());
}

void setup_ui_control()
{
    ui_control_tcl_output();

    ObjectFactory<UIControl> obj("ui");

    obj.addMethod("open", &UIControl::m_open);
}
