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
#include "ceammc_format.h"
#include "ceammc_string.h"
#include "fmt/core.h"

namespace {
inline std::string space(int s = 4)
{
    return std::string(s, ' ');
}
}

namespace ceammc {

TclPropDialogGenerator::TclPropDialogGenerator(const BaseObject* obj)
    : obj_(obj)
{
}

std::string TclPropDialogGenerator::generate() const
{
    return fmt::format("proc {0} {{id props}} {{\n"
                       "{1}"
                       "}}\n",
        procName(), procBody());
}

std::string TclPropDialogGenerator::procName() const
{
    std::string name = string::replace_all(obj_->className()->s_name, ".", "_");
    return fmt::format("ceammc_dialog_{0}", name);
}

std::string TclPropDialogGenerator::procArgs() const
{
    std::string res;

    for (auto& p : obj_->getProperties()) {
        res += '{';
        res += p->name()->s_name[1];
        res += '}';
        res += ' ';
    }

    if (!res.empty() && res.back() == ' ')
        res.pop_back();

    return res;
}

std::string TclPropDialogGenerator::procBody() const
{
    std::string res = procBodyInit();

    res += R"(

    frame $w.f
    pack $w.f -expand yes -fill both -padx 1 -pady 1
    grid rowconfig    $w.f 0 -weight 1 -minsize 0
    grid columnconfig $w.f 0 -weight 1 -minsize 0
)";

    int row = 0;
    for (auto& p : obj_->getProperties()) {
        switch (p->visibility()) {
        case PropValueVis::HIDDEN:
        case PropValueVis::INTERNAL:
            // ignore hidden properties
            continue;
            break;
        default:
            break;
        }

        res += grid(row, 0, label(row, p->name()->s_name), "w");

        switch (p->type()) {
        case PropValueType::BOOLEAN: {
            bool v = false;
            if (p->getBool(v)) {
                res += entryBool(row, v, p->infoT());
            }
        } break;
        case PropValueType::INTEGER: {
            t_int v = 0;
            if (p->getInt(v)) {
                res += entryInt(row, v, p->infoT());
            }
        } break;
        case PropValueType::FLOAT: {
            t_float v = 0;
            if (p->getFloat(v)) {
                res += entryFloat(row, v, p->infoT());
            }

        } break;
        case PropValueType::SYMBOL:
            break;
        case PropValueType::ATOM:
            break;
        case PropValueType::LIST:
            break;
        default:
            break;
        }

        res += grid(row, 1, widgetId(row), "news");

        row++;
    }

    return res;
}

std::string TclPropDialogGenerator::entryInt(int row, t_int value, const PropertyInfo& info) const
{
    auto res = propVar(row, info.name());
    res += spinbox(row, info);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::entryFloat(int row, t_float value, const PropertyInfo& info) const
{
    auto res = propVar(row, info.name());
    res += spinbox(row, info);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::entryBool(int row, bool value, const PropertyInfo& info) const
{
    auto res = propVar(row, info.name());
    res += checkbox(row);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::propVar(int row, t_symbol* name) const
{
    return fmt::format("{0}set {1} [dict get $props \"{2}\"]\n",
        space(),
        propVarName(row),
        name->s_name);
}

std::string TclPropDialogGenerator::propVarName(int row) const
{
    return fmt::format("::ceammc::dialog_vars(v{0}_{1})",
        static_cast<const void*>(this),
        row);
}

std::string TclPropDialogGenerator::checkbox(int row) const
{
    return fmt::format("{0}ttk::checkbutton {1} -variable {2}\n",
        space(),
        widgetId(row),
        propVarName(row));
}

std::string TclPropDialogGenerator::spinbox(int row, const PropertyInfo& info) const
{
    const auto IND = space(4);

    auto res = fmt::format("{0}ttk::spinbox {1} -textvariable {2}\n",
        IND,
        widgetId(row),
        propVarName(row));

    if (info.hasConstraintsMin()) {
        if (info.isInt()) {
            auto v = info.minInt();
            switch (info.constraints()) {
            case PropValueConstraints::OPEN_CLOSED_RANGE: // fallthru
            case PropValueConstraints::GREATER_THEN:
            case PropValueConstraints::OPEN_RANGE:
                v++;
                break;
            default:
                break;
            }

            res += fmt::format("{0}{1} configure -from {2}\n", IND, widgetId(row), v);
        } else if (info.isFloat()) {
            auto v = info.minFloat();
            switch (info.constraints()) {
            case PropValueConstraints::OPEN_CLOSED_RANGE: // fallthru
            case PropValueConstraints::GREATER_THEN:
            case PropValueConstraints::OPEN_RANGE:
                v += 0.001;
                break;
            default:
                break;
            }

            res += fmt::format("{0}{1} configure -from {2}\n", IND, widgetId(row), v);
        }
    }

    if (info.hasConstraintsMax()) {
        if (info.isInt()) {
            auto v = info.maxInt();
            switch (info.constraints()) {
            case PropValueConstraints::CLOSED_OPEN_RANGE: // fallthru
            case PropValueConstraints::LESS_THEN:
            case PropValueConstraints::OPEN_RANGE:
                v--;
                break;
            default:
                break;
            }

            res += fmt::format("{0}{1} configure -to {2}\n", IND, widgetId(row), v);
        } else if (info.isFloat())
            res += fmt::format("{0}{1} configure -to {2}\n", IND, widgetId(row), info.maxFloat());
    }

    if (info.hasEnumLimit()) {
        res += fmt::format("{0}{1} configure -values {2}\n", IND, widgetId(row), to_string(info.enumValues()));
    }

    return res;
}

std::string TclPropDialogGenerator::label(int row, const std::string& text)
{
    return fmt::format(R"([ttk::label $w.f.l{0} -text "{1}"])", row, text);
}

std::string TclPropDialogGenerator::widgetId(int row)
{
    return fmt::format("$w.f.x{0}", row);
}

std::string TclPropDialogGenerator::widgetState(int row, PropValueAccess state)
{
    switch (state) {
    case PropValueAccess::READONLY: // fallthru
    case PropValueAccess::INITONLY:
        return fmt::format("{0}{1} configure -state readonly\n", space(), widgetId(row));
        break;
    case PropValueAccess::READWRITE:
    default:
        return {};
    }
}

std::string TclPropDialogGenerator::grid(int row, int col, const std::string& widget, const char* sticky)
{
    return fmt::format("{0}grid {1} -in $w.f -padx 1 -pady 1 -row {2} -column {3} -sticky {4}\n",
        space(),
        widget,
        row,
        col,
        sticky);
}

std::string TclPropDialogGenerator::procBodyInit() const
{
    std::string res;

    res = fmt::format(R"(
    set w $id
    catch {{destroy $w}}
    toplevel $w
    wm title $w [_ "\[{0}\] object properties"]
    wm iconname $w "form"
    bind $w <Key-Escape> "destroy $w; break"
    raise [winfo toplevel $w]
)",
        obj_->className()->s_name);

    return res;
}

std::string TclPropDialogGenerator::callProc() const
{
    std::string res = fmt::format("{0} %s ", procName());
    res += "[dict create ";

    for (auto& p : obj_->getProperties()) {
        res += p->name()->s_name;
        res += ' ';
        res += to_string(p->get());
        res += ' ';
    }

    if (res.back() == ' ')
        res.pop_back();

    res += ']';
    res += '\n';
    return res;
}

} // namespace ceammc
