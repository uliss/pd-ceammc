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

constexpr auto COL_PROP_NAME = 0;
constexpr auto COL_WIDGET = 1;
constexpr auto COL_WIDGET2 = 2;
constexpr auto COL_PROP_TYPE = 3;
constexpr auto COL_PROP_RESET = 4;
constexpr auto COL_PROP_DEFAULT = 5;

TclPropDialogGenerator::TclPropDialogGenerator(const BaseObject* obj)
    : obj_(obj)
{
}

std::string TclPropDialogGenerator::generate() const
{
    auto class_name = makeClassName();

    return fmt::format("proc {0} {{id props}} {{\n"
                       "    set args [list $id .prop_set]\n"
                       "    dict for {{k v}} $props {{lappend args $k [::ceammc::dialog::escape2pd $v]}}\n"
                       "    pdsend $args\n"
                       "}}\n"
                       "proc {1} {{id wid var prop value}} {{pdsend [list $id .prop_validate $var $wid $prop $value]\n"
                       "}}\n"
                       "proc {2} {{id props}} {{\n"
                       "{3}"
                       "}}\n",
        okProcName(class_name.c_str()),
        validateProcName(class_name.c_str()),
        procName(class_name.c_str()),
        procBody());
}

std::string TclPropDialogGenerator::procName(const char* className)
{
    return fmt::format("ceammc_dialog_{0}", className);
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
        res += grid(row, COL_PROP_NAME, propLabel(row, name), "w");

        switch (p->type()) {
        case PropValueType::BOOLEAN: {
            res += entryBool(row, p->info());
        } break;
        case PropValueType::INTEGER: {
            res += entryInt(row, p->info());
        } break;
        case PropValueType::FLOAT: {
            res += entryFloat(row, p->info());
        } break;
        case PropValueType::SYMBOL: {
            res += entrySymbol(row, p->info());
        } break;
        case PropValueType::ATOM:
            break;
        case PropValueType::LIST:
            break;
        default:
            break;
        }

        res += grid(row, COL_WIDGET, widgetId(row), "news");

        auto unit_label = unitsLabel(row, p->info());
        if (!unit_label.empty())
            res += grid(row, COL_PROP_TYPE, unit_label, "w");

        res += grid(row, COL_PROP_RESET, fmt::format("[button $w.f.btn_reset{0} -text [_ Reset]]", row), "e");
        res += fmt::format("{0}::ceammc_tooltip $w.f.btn_reset{1} [_ {{Reset to default}}]\n", space(), row);
        res += grid(row, COL_PROP_DEFAULT, fmt::format("[button $w.f.btn_default{0} -text [_ Default]]", row), "e");

        last_row = row;
    });

    res += buttons(last_row + 1);

    return res;
}

std::string TclPropDialogGenerator::entryInt(int row, const PropertyInfo& info)
{
    auto res = spinbox(row, info);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::entrySymbol(int row, const PropertyInfo& info)
{
    std::string res;

    if (info.hasEnumLimit()) {
        res += combobox(row, info);
    } else if (info.view() == PropValueView::FILEPATH) {
        res += pathentry(row, info);
    } else {
        res += textentry(row, info);
    }

    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::entryFloat(int row, const PropertyInfo& info)
{
    auto res = spinbox(row, info);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::entryBool(int row, const PropertyInfo& info)
{
    auto res = checkbox(row, info);
    res += widgetState(row, info.access());
    return res;
}

std::string TclPropDialogGenerator::checkbox(int propIdx, const PropertyInfo& info)
{
    const auto indent = space(4);
    const auto wid = widgetId(propIdx);
    const auto prop_name = info.name()->s_name;

    std::string res;

    res += fmt::format("{0}set {1} [dict get $props {{{2}}}]\n", indent, propVarName(propIdx), prop_name);
    res += fmt::format("{0}ttk::checkbutton {1} -variable {2}\n", indent, wid, propVarName(propIdx));
    res += fmt::format("{0}{1} configure -command \"dict set {2} {3} \\${4}\"\n", indent, wid, propsDictVar(), prop_name, propVarName(propIdx));

    return res;
}

std::string TclPropDialogGenerator::textentry(int propIdx, const PropertyInfo& info)
{
    const auto indent = space(4);
    const auto wid = widgetId(propIdx);
    const auto prop_name = info.name()->s_name;

    std::string res;

    res += fmt::format("{0}ttk::entry {1} -validate key\n", indent, wid);
    res += fmt::format("{0}{1} insert end [dict get $props {{{2}}}]\n", indent, wid, prop_name);
    res += fmt::format("{0}{1} configure -validatecommand \"dict set {2} {3} %P; return 1;\"\n",
        indent, wid, propsDictVar(), prop_name);

    return res;
}

std::string TclPropDialogGenerator::pathentry(int propIdx, const PropertyInfo& info)
{
    const auto indent = space(4);
    const auto wid = widgetId(propIdx);
    const auto prop_name = info.name()->s_name;

    std::string res;

    res += fmt::format("{0}set {1} [dict get $props {{{2}}}]\n", indent, propVarName(propIdx), prop_name);
    res += fmt::format("{0}ttk::entry {1} -validate key -textvariable {2}\n", indent, wid, propVarName(propIdx));
    res += fmt::format("{0}{1} insert end [dict get $props {{{2}}}]\n", indent, wid, prop_name);
    res += fmt::format("{0}{1} configure -validatecommand \"dict set {2} {3} %P; return 1;\"\n",
        indent, wid, propsDictVar(), prop_name);

    res += fmt::format("{0}ttk::button {1}_btn"
                       " -text [_ Choose]"
                       " -command \"::ceammc::dialog::open_file [dict get $props {{{2}}}] {1}\"\n",
        indent, wid, prop_name);

    res += grid(propIdx, COL_WIDGET2, fmt::format("{}_btn", wid), "n");

    return res;
}

std::string TclPropDialogGenerator::propsDictVar()
{
    return fmt::format("::ceammc::dialog_data(v$id)");
}

std::string TclPropDialogGenerator::setDialogValue(const char* prop, const char* val)
{
    return fmt::format("dict set ${0} {1} {2}; puts {2};", propsDictVar(), prop, val);
}

std::string TclPropDialogGenerator::spinbox(int propIdx, const PropertyInfo& info)
{
    constexpr double MIN_VALUE = -9999999999;
    constexpr double MAX_VALUE = +9999999999;

    const auto indent = space(4);
    const auto id = widgetId(propIdx);
    const auto prop_name = info.name()->s_name;

    auto res = fmt::format("{0}ttk::spinbox {1}\n",
        indent,
        id);

    res += fmt::format("{0}{1} set [dict get $props {{{2}}}]\n", indent, id, prop_name);
    res += fmt::format("{0}{1} configure -command \"dict set {2} {3} \\[{1} get\\]\"\n", indent, id, propsDictVar(), prop_name);

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
            indent, id);

        res += fmt::format("{0}{1} configure -invalidcommand {{%W set %s}}\n",
            indent, id);
    }

    if (info.isFloat()) {
        res += fmt::format("{0}{1} configure"
                           " -validate key"
                           " -validatecommand {{string is double %P}}\n",
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
        indent, id, propsDictVar(), prop_name);

    return res;
}

std::string TclPropDialogGenerator::combobox(int propIdx, const PropertyInfo& info)
{
    const auto indent = space(4);
    const auto wid = widgetId(propIdx);
    const auto prop_name = info.name()->s_name;

    std::string res;

    res += fmt::format("{0}ttk::combobox {1} -state readonly\n", indent, wid);
    res += fmt::format("{0}{1} set [dict get $props {{{2}}}]\n", indent, wid, prop_name);
    res += fmt::format("{0}bind {1} <<ComboboxSelected>> \"dict set {2} {{{3}}} \\[%W get\\]\"\n",
        indent, wid, propsDictVar(), prop_name);

    if (info.hasEnumLimit()) {
        res += fmt::format("{0}{1} configure -values {2}\n", indent, wid, list2tcl(info.enumValues()));
    }

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
        return propLabel(1000 + row, fmt::format("{1} ({0})", units, propType(info)));
    else
        return propLabel(1000 + row, fmt::format("{}", propType(info)));
}

std::string TclPropDialogGenerator::propType(const PropertyInfo& info)
{
    switch (info.type()) {
    case PropValueType::BOOLEAN:
        return "bool";
    case PropValueType::INTEGER:
        return "int";
    case PropValueType::FLOAT:
        return "float";
    case PropValueType::SYMBOL:
        return "symbol";
    case PropValueType::ATOM:
        return "atom";
    case PropValueType::LIST:
        return "list";
        break;
    default:
        return "?";
    }
}

std::string TclPropDialogGenerator::list2tcl(const AtomListView& lv)
{
    if (lv.size() == 1)
        return atom2tcl(lv[0]);
    else {
        std::string res = "[list";
        for (auto& a : lv) {
            res += ' ';
            res += atom2tcl(a);
        }

        res += ']';
        return res;
    }
}

std::string TclPropDialogGenerator::atom2tcl(const Atom& a)
{
    switch (a.type()) {
    case Atom::NONE:
        return "#null";
    case Atom::FLOAT:
        return fmt::format("{}", a.asT<t_float>());
    case Atom::PROPERTY: // fallthru
    case Atom::SYMBOL: {

        auto str = a.asT<t_symbol*>()->s_name;
        auto len = strlen(str);

        std::string res = "{";
        for (size_t i = 0; i < len; i++) {
            auto c = str[i];
            switch (c) {
            case '{':
                res += "\\{";
                break;
            case '}':
                res += "\\}";
                break;
                // case '$':
                //     res += "\\$";
                break;
            default:
                res += c;
                break;
            }
        }

        res += '}';
        return res;

    } break;
    // fallthru
    case Atom::DATA:
    case Atom::POINTER:
    case Atom::SEMICOLON:
    case Atom::COMMA:
    case Atom::DOLLAR:
    case Atom::DOLLAR_SYMBOL:
    default:
        return "?";
    }
}

bool TclPropDialogGenerator::setList(Property* p, const AtomListView& lv)
{
    switch (p->type()) {
    case PropValueType::BOOLEAN:
    case PropValueType::INTEGER:
    case PropValueType::FLOAT:
        return p->setList(lv);
        break;

    case PropValueType::SYMBOL: {
        auto str = to_string(lv);
        if (!str.empty() && str.front() == '{' && str.back() == '}') {
            str.pop_back();
            str = str.substr(1);
        }

        LIB_DBG << str;
        p->setList(Atom(gensym(str.c_str())));
    } break;
    case PropValueType::ATOM:
    case PropValueType::LIST:
        break;
    }

    return false;
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
        obj_->className()->s_name, propsDictVar());

    // image create photo icon_reset -data iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAABhklEQVRIS+2VzytFQRTH383KwkKSkJQfC1lIiighxeaFJVn5H7D2D/BXWLCRDfIjPSlF8ewoEVG8srBQNorPV3Pyuu51p1d3I1OfZppz5nzPnDszN8ik3IKU42f+vkA5JVyAaaj3LGcBvx2Yh6ffSlSJwxb0eAYOu60yMRknUItxF9qVBYzDiafQAH45eIbqKIEWDPvQAJcwAveewc3tww2CsEA3hk2ochmP0r+YM/0B6LtkQbVWq4ENeIVBNxcpoEzXXQB9pAl4K8pcyZxCJ1xDv7Md0jfDGXTFCSgjBS+DZZiB96LgNlS2R9AEd26ykf4G+sB29WMHtxjluASzYA4RGl8lOXb+sktIJ82Cay5WYBHjXBoCYwRdS7NE2pbPR87j1wFXoPOuppPVCuegAxBZIqt10jHVZauAIXh0i3Qpc6DjbLc+9h5oTaoXzXZSx2AP2uDBle/CjAn9sFsb+1TYej12uqG9noHDbitMTCX9cEp9rrcJrue6kCRQYvLfy/4FEkv4Ccw8XBl0/I8MAAAAAElFTkSuQmCC

    return res;
}

std::string TclPropDialogGenerator::propVarName(int propIdx)
{
    return fmt::format("::ceammc_prop_vars(obj${{id}}_prop{})", propIdx);
}

std::string TclPropDialogGenerator::callProc() const
{
    std::string res = fmt::format("{0} %s ", procName(makeClassName().c_str()));
    res += "[dict create ";

    string::StaticString out;

    for (auto& p : obj_->getProperties()) {
        res += p->name()->s_name;
        res += ' ';
        if (p->isNumeric())
            res += to_string(p->get());
        else {
            res += list2tcl(p->get());
        }

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
    res += fmt::format("{0}ttk::button $w.f.btn_ok -text [_ \"Ok\"] -command \"{1} $id \\${2}; destroy $w\"\n",
        indent,
        okProcName(makeClassName().c_str()),
        propsDictVar());

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
                       " -row {1} -column 3 -columnspan 2 -sticky e\n",
        indent, row);

    return res;
}

std::string TclPropDialogGenerator::makeClassName() const
{
    return string::replace_all(obj_->className()->s_name, ".", "_");
}

std::string TclPropDialogGenerator::okProcName(const char* className)
{
    return procName(className) + "_ok";
}

std::string TclPropDialogGenerator::validateProcName(const char* className)
{
    return procName(className) + "_validate";
}

} // namespace ceammc
