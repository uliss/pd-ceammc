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

inline const char* unit2ui_suffix(ceammc::PropValueUnits unit)
{
    switch (unit) {
    case ceammc::PropValueUnits::NONE:
        return nullptr;
    case ceammc::PropValueUnits::MSEC:
        return "ms";
    case ceammc::PropValueUnits::SEC:
        return "s";
    case ceammc::PropValueUnits::SAMP:
        return "samp";
    case ceammc::PropValueUnits::DB:
        return "db";
    case ceammc::PropValueUnits::DEG:
        return "deg";
    case ceammc::PropValueUnits::RAD:
        return "rad";
    case ceammc::PropValueUnits::HZ:
        return "Hz";
    case ceammc::PropValueUnits::PERCENT:
        return "%";
    case ceammc::PropValueUnits::CENT:
        return "cent";
    case ceammc::PropValueUnits::SEMITONE:
        return "semitone";
    case ceammc::PropValueUnits::TONE:
        return "tone";
    case ceammc::PropValueUnits::BPM:
        return "bpm";
    // case ceammc::PropValueUnits::SMPTE:
    case ceammc::PropValueUnits::PIXEL:
        return "px";
    case ceammc::PropValueUnits::MICROSEC:
        return "usec";
    case ceammc::PropValueUnits::NANOSEC:
        return "nsec";
    case ceammc::PropValueUnits::CENTIMETER:
        return "cm";
    case ceammc::PropValueUnits::MILLIMETER:
        return "mm";
    case ceammc::PropValueUnits::MINUTE:
        return "min";
    case ceammc::PropValueUnits::HOUR:
        return "hour";
    case ceammc::PropValueUnits::DAY:
        return "day";
    default:
        return nullptr;
    }
}
} // namespace

namespace ceammc {

TclPropDialogGenerator::TclPropDialogGenerator(const BaseObject* obj)
    : obj_(obj)
{
}

std::string TclPropDialogGenerator::generate() const
{
    return fmt::format("proc {0} {{id props}} {{\n"
                       "    set args [list $id .prop_set]\n"
                       "    dict for {{k v}} $props {{\n"
                       "        lappend args $k $v\n"
                       "    }}\n"
                       "    pdsend $args\n"
                       "}}\n"
                       "proc {1} {{id wid var prop value}} {{pdsend [list $id .prop_validate $var $wid $prop $value]\n"
                       "}}\n"
                       "proc {2} {{id props}} {{\n"
                       "{3}"
                       "}}\n",
        okProcName(), validatePropName(), procName(), procBody());
}

std::string TclPropDialogGenerator::procName() const
{
    std::string name = string::replace_all(obj_->className()->s_name, ".", "_");
    return fmt::format("ceammc_dialog_{0}", name);
}

void TclPropDialogGenerator::foreachProperty(const std::function<void(const char*, int, const Property*)>& cb) const
{
    int prop_idx = 0;

    for (auto& p : obj_->getProperties()) {
        switch (p->visibility()) {
        case PropValueVis::HIDDEN:
        case PropValueVis::INTERNAL:
            // ignore hidden properties
            continue;
            break;
        default:
            if (p != nullptr)
                cb(p->name()->s_name, prop_idx++, p);
            break;
        }
    }
}

std::string TclPropDialogGenerator::procBody() const
{
    std::string res = procBodyInit();

    res += R"(

    frame $w.f -padx 10 -pady 10
    pack $w.f -expand yes -fill both -padx 1 -pady 1
    grid rowconfig    $w.f 0 -weight 1 -minsize 0
    grid columnconfig $w.f 0 -weight 1 -minsize 0
)";

    int last_row = 0;
    foreachProperty([&res, this, &last_row](const char* name, int row, const Property* p) {
        res += grid(row, 0, propLabel(row, name), "w");

        switch (p->type()) {
        case PropValueType::BOOLEAN: {
            bool v = false;
            if (p->getBool(v)) {
                res += entryBool(row, v, p->info());
            }
        } break;
        case PropValueType::INTEGER: {
            t_int v = 0;
            if (p->getInt(v)) {
                res += entryInt(row, v, p->info());
            }
        } break;
        case PropValueType::FLOAT: {
            t_float v = 0;
            if (p->getFloat(v)) {
                res += entryFloat(row, v, p->info());
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

        auto unit_label = unitsLabel(row, p->info());
        if (!unit_label.empty())
            res += grid(row, 2, unit_label, "w");

        res += grid(row, 3, fmt::format("[button $w.f.btn_reset{0} -text Reset]", row), "e");
        res += fmt::format("{0}::ceammc_tooltip $w.f.btn_reset{1} [_ {{Reset to default}}]\n", space(), row);

        last_row = row;
    });

    res += buttons(last_row + 1);

    return res;
}

std::string TclPropDialogGenerator::entryInt(int row, t_int value, const PropertyInfo& info)
{
    auto res = spinbox(row, info);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::entryFloat(int row, t_float value, const PropertyInfo& info)
{
    auto res = spinbox(row, info);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::entryBool(int row, bool value, const PropertyInfo& info)
{
    auto res = checkbox(row);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::checkbox(int row)
{
    return fmt::format("{0}ttk::checkbutton {1}\n",
        space(),
        widgetId(row));
}

std::string TclPropDialogGenerator::dialogDataVar()
{
    return fmt::format("::ceammc::dialog_data(v$id)");
}

std::string TclPropDialogGenerator::getDialogValue(const char* prop)
{
    return fmt::format("dict get ${0} {1}", dialogDataVar(), prop);
}

std::string TclPropDialogGenerator::setDialogValue(const char* prop, const char* val)
{
    return fmt::format("dict set ${0} {1} {2}; puts {2};", dialogDataVar(), prop, val);
}

std::string TclPropDialogGenerator::spinbox(int row, const PropertyInfo& info)
{
    constexpr double MIN_VALUE = -9999999999;
    constexpr double MAX_VALUE = +9999999999;

    const auto indent = space(4);
    const auto id = widgetId(row);

    auto res = fmt::format("{0}ttk::spinbox {1}\n",
        indent,
        id);

    res += fmt::format("{0}{1} set [dict get $props {{{2}}}]\n", indent, id, info.name()->s_name);
    res += fmt::format("{0}{1} configure -command \"dict set {2} {3} \\[{1} get\\]\"\n", indent, id, dialogDataVar(), info.name()->s_name);

    if (info.hasEnumLimit()) {
        res += fmt::format("{0}{1} configure -values {2}\n", indent, id, to_string(info.enumValues()));
    } else {
        double vmin = MIN_VALUE;
        double vmax = MAX_VALUE;

        if (info.hasConstraintsMin()) {
            if (info.isInt()) {
                vmin = info.minInt();
                switch (info.constraints()) {
                case PropValueConstraints::OPEN_CLOSED_RANGE: // fallthru
                case PropValueConstraints::GREATER_THEN:
                case PropValueConstraints::OPEN_RANGE:
                    vmin++;
                    break;
                default:
                    break;
                }
            } else if (info.isFloat()) {
                vmin = info.minFloat();
                switch (info.constraints()) {
                case PropValueConstraints::OPEN_CLOSED_RANGE: // fallthru
                case PropValueConstraints::GREATER_THEN:
                case PropValueConstraints::OPEN_RANGE:
                    vmin += 0.001;
                    break;
                default:
                    break;
                }
            }
        }

        if (info.hasConstraintsMax()) {
            if (info.isInt()) {
                vmax = info.maxInt();
                switch (info.constraints()) {
                case PropValueConstraints::CLOSED_OPEN_RANGE: // fallthru
                case PropValueConstraints::LESS_THEN:
                case PropValueConstraints::OPEN_RANGE:
                    vmax--;
                    break;
                default:
                    break;
                }
            } else if (info.isFloat()) {
                vmax = info.maxFloat();
                switch (info.constraints()) {
                case PropValueConstraints::CLOSED_OPEN_RANGE: // fallthru
                case PropValueConstraints::LESS_THEN:
                case PropValueConstraints::OPEN_RANGE:
                    vmax -= 0.001;
                    break;
                default:
                    break;
                }
            }
        }

        res += fmt::format("{0}{1} configure -from {2} -to {3}\n",
            indent, id, vmin, vmax);

        if (info.step() > 0 && info.step() != 1) {
            res += fmt::format("{0}{1} configure -increment {2}\n", indent, id, info.step());
        }
    }

    if (info.isInt()) {
        res += fmt::format("{0}{1} configure"
                           " -validate key"
                           " -validatecommand {{string is int %P}}\n",
            indent, id, setDialogValue(info.name()->s_name, "%P"));

        res += fmt::format("{0}{1} configure -invalidcommand {{%W set %s}}\n",
            indent, id);
    }

    if (info.isFloat()) {
        res += fmt::format("{0}{1} configure"
                           " -validate key"
                           " -validatecommand {{string is double %P}}",
            indent, id);

        res += fmt::format("{0}{1} configure -invalidcommand {{%W set %s}}\n",
            indent, id);
    }

    if (!info.equalUnit(PropValueUnits::NONE)) {
        int unit_count = 0;
        const char* unit = nullptr;
        info.unitsIterate([&unit_count, &unit](PropValueUnits u) {
            unit_count++;
            unit = unit2ui_suffix(u);
        });
    }

    res += fmt::format("{0}::ceammc::ui::bindMouseWheel {1} {{::ceammc::ui::spinboxScroll %W}}\n", indent, id);
    res += fmt::format("{0}bind {1} <Return> \"ceammc_dialog_bitmap_validate $id %W {2} {3} \\[%W get\\]\"\n",
        indent, id, dialogDataVar(), info.name()->s_name);

    return res;
}

std::string TclPropDialogGenerator::propLabel(int row, const std::string& text)
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

std::string TclPropDialogGenerator::unitsLabel(int row, const PropertyInfo& info)
{
    int unit_count = 0;
    std::string units;
    info.unitsIterate([&unit_count, &units](PropValueUnits u) { //
        if (unit_count > 0)
            units += ' ';

        units += unit2ui_suffix(u);
        unit_count++;
    });

    if (unit_count == 1)
        return propLabel(1000 + row, units);
    else
        return {};
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

    set {1} $props
)",
        obj_->className()->s_name, dialogDataVar());

    // image create photo icon_reset -data iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAABhklEQVRIS+2VzytFQRTH383KwkKSkJQfC1lIiighxeaFJVn5H7D2D/BXWLCRDfIjPSlF8ewoEVG8srBQNorPV3Pyuu51p1d3I1OfZppz5nzPnDszN8ik3IKU42f+vkA5JVyAaaj3LGcBvx2Yh6ffSlSJwxb0eAYOu60yMRknUItxF9qVBYzDiafQAH45eIbqKIEWDPvQAJcwAveewc3tww2CsEA3hk2ochmP0r+YM/0B6LtkQbVWq4ENeIVBNxcpoEzXXQB9pAl4K8pcyZxCJ1xDv7Md0jfDGXTFCSgjBS+DZZiB96LgNlS2R9AEd26ykf4G+sB29WMHtxjluASzYA4RGl8lOXb+sktIJ82Cay5WYBHjXBoCYwRdS7NE2pbPR87j1wFXoPOuppPVCuegAxBZIqt10jHVZauAIXh0i3Qpc6DjbLc+9h5oTaoXzXZSx2AP2uDBle/CjAn9sFsb+1TYej12uqG9noHDbitMTCX9cEp9rrcJrue6kCRQYvLfy/4FEkv4Ccw8XBl0/I8MAAAAAElFTkSuQmCC

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

std::string TclPropDialogGenerator::buttons(int row) const
{
    const auto indent = space();

    std::string res;

    res += fmt::format("{0}ttk::button $w.f.btn_cancel -text [_ \"Cancel\"] -command \"destroy $w\"\n", indent);
    res += fmt::format("{0}ttk::button $w.f.btn_apply -text [_ \"Apply\"]\n", indent);
    res += fmt::format("{0}ttk::button $w.f.btn_ok -text [_ \"Ok\"] -command \"{1} $id \\${2}\"\n", indent, okProcName(), dialogDataVar());

    res += fmt::format("{0}grid $w.f.btn_cancel -in $w.f"
                       " -padx 1 -pady 1"
                       " -row {1} -column 0 -sticky w\n",
        indent, row);

    res += fmt::format("{0}grid $w.f.btn_apply -in $w.f"
                       " -padx 1 -pady 1"
                       " -row {1} -column 1 -sticky w\n",
        indent, row);

    res += fmt::format("{0}grid $w.f.btn_ok -in $w.f"
                       " -padx 1 -pady 1"
                       " -row {1} -column 2 -columnspan 2 -sticky e\n",
        indent, row);

    return res;
}

std::string TclPropDialogGenerator::okProcName() const
{
    return procName() + "_ok";
}

std::string TclPropDialogGenerator::validatePropName() const
{
    return procName() + "_validate";
}

} // namespace ceammc
