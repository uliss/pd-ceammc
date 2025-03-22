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
#include "ceammc_properties_object.h"
#include "fmt/core.h"
#include "test_macro.h"

#include "catch.hpp"

using namespace ceammc;

class PropObject : public PropertiesObject<BaseObject> {
public:
    explicit PropObject(const char* name)
        : PropertiesObject<BaseObject>(PdArgs(L(), gensym(name), nullptr, &s_))
    {
        addProperty(new FloatProperty("@f", 1.0));
    }
};

TEST_CASE("tcl_prop_dialog_generator", "[core]")
{
    SECTION("procName")
    {
        PropObject obj("test.object");

        TclPropDialogGenerator gen(&obj);
        REQUIRE(gen.procName() == "ceammc_dialog_test_object");
        REQUIRE(gen.procArgs() == "{f}");
        REQUIRE(gen.procBodyInit() == R"(
    set w $id
    catch {destroy $w}
    toplevel $w
    wm title $w [_ "\[test.object\] object properties"]
    wm iconname $w "form"
    bind $w <Key-Escape> "destroy $w; break"
    raise [winfo toplevel $w]
)");
        REQUIRE(gen.callProc() == "ceammc_dialog_test_object %s [dict create @f 1]\n");
    }

    SECTION("spinbox")
    {
        PropObject obj("test.object");

        TclPropDialogGenerator gen(&obj);
        PropertyInfo pi("name", PropValueType::INTEGER);

        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -9999999999 -to 9999999999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::GREATER_EQUAL);
        REQUIRE(pi.setMinInt(-100));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -100 -to 9999999999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::GREATER_THEN);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -99 -to 9999999999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::LESS_EQUAL);
        REQUIRE(pi.setMaxInt(100));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -9999999999 -to 100\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::LESS_THEN);
        REQUIRE(pi.setMaxInt(100));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -9999999999 -to 99\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::CLOSED_RANGE);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -100 -to 100\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::OPEN_RANGE);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -99 -to 99\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::OPEN_CLOSED_RANGE);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -99 -to 100\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::CLOSED_OPEN_RANGE);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -100 -to 99\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        REQUIRE(pi.setStep(10));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -100 -to 99\n"
                           "    $w.f.x0 configure -increment 10\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        REQUIRE(pi.setStep(0));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -100 -to 99\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::ENUM);
        REQUIRE(pi.addEnums({ 1, 2, 4, 8, 16 }));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -values 1 2 4 8 16\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is int %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi = PropertyInfo("name", PropValueType::FLOAT);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -9999999999 -to 9999999999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::GREATER_EQUAL);
        REQUIRE(pi.setMinFloat(-100));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -100 -to 9999999999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::GREATER_THEN);
        REQUIRE(pi.setMinFloat(-100));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -99.999 -to 9999999999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::LESS_EQUAL);
        REQUIRE(pi.setMaxFloat(100));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -9999999999 -to 100\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::LESS_THEN);
        REQUIRE(pi.setMaxFloat(100));
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -9999999999 -to 99.999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::CLOSED_RANGE);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -100 -to 100\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::OPEN_RANGE);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -99.999 -to 99.999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::CLOSED_OPEN_RANGE);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -100 -to 99.999\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));

        pi.setConstraints(PropValueConstraints::OPEN_CLOSED_RANGE);
        REQUIRE(gen.spinbox(0, pi)
            == fmt::format("    ttk::spinbox $w.f.x0\n"
                           "    $w.f.x0 set [dict get $props {{name}}]\n"
                           "    $w.f.x0 configure -from -99.999 -to 100\n"
                           "    $w.f.x0 configure -validate key -validatecommand {{string is double %P}} -invalidcommand {{%W set %s}}\n"
                           "    ::ceammc::ui::bindMouseWheel $w.f.x0 {{::ceammc::ui::spinboxScroll %W}}\n"));
    }
}
