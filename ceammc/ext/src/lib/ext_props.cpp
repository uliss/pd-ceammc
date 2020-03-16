/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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

#include "../mod_init.h"
#include "ceammc_format.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "stk/stk/include/Stk.h"

extern "C" {
extern "C" void pd_init();
#include "s_stuff.h"
}

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

static t_symbol* any = &s_anything;

t_class* ceammc_class = nullptr;

using namespace std;
using namespace ceammc;

static const char* to_string(PropValueType t)
{
    switch (t) {
    case PropValueType::BOOLEAN:
        return "bool";
    case PropValueType::FLOAT:
        return "float";
    case PropValueType::INTEGER:
        return "int";
    case PropValueType::LIST:
        return "list";
    case PropValueType::SYMBOL:
        return "symbol";
    case PropValueType::VARIANT:
        return "atom";
    }
}

static const char* to_string(PropValueView v)
{
    switch (v) {
    case PropValueView::SLIDER:
        return "slider";
    case PropValueView::KNOB:
        return "knob";
    case PropValueView::TOGGLE:
        return "toggle";
    case PropValueView::COLOR:
        return "color";
    case PropValueView::NUMBOX:
        return "numbox";
    default:
        return "entry";
    }
}

static const char* to_string(PropValueUnits u)
{
    switch (u) {
    case PropValueUnits::MSEC:
        return "millisecond";
    case PropValueUnits::SEC:
        return "second";
    case PropValueUnits::DB:
        return "decibel";
    case PropValueUnits::HZ:
        return "herz";
    case PropValueUnits::PERCENT:
        return "percent";
    case PropValueUnits::RAD:
        return "radian";
    case PropValueUnits::DEG:
        return "degree";
    case PropValueUnits::SEMITONE:
        return "semitone";
    case PropValueUnits::CENT:
        return "cent";
    case PropValueUnits::TONE:
        return "tone";
    case PropValueUnits::SAMP:
        return "sample";
    default:
        return "";
    }
}

static const char* to_string(PropValueAccess v)
{
    switch (v) {
    case PropValueAccess::READWRITE:
        return "readwrite";
    case PropValueAccess::READONLY:
        return "readonly";
    case PropValueAccess::INITONLY:
        return "initonly";
    }
}

static const char* to_string(PropValueVis v)
{
    switch (v) {
    case PropValueVis::PUBLIC:
        return "public";
    case PropValueVis::HIDDEN:
        return "hidden";
    case PropValueVis::INTERNAL:
        return "internal";
    }
}

static std::string to_string2(const AtomList& lst)
{
    std::string res;
    res += "[ ";
    for (size_t i = 0; i < lst.size(); i++) {
        if (i != 0)
            res += ", ";

        if (lst[i].isSymbol()) {
            res += '"';
            res += to_string(lst[i]);
            res += '"';
        } else
            res += to_string(lst[i]);
    }
    res += " ]";
    return res;
}

static void printInfo(std::ostream& os, const PropertyInfo& pi)
{
    os << "  \"" << pi.name()->s_name << "\": {\n";
    os << "    \"type\": \"" << to_string(pi.type()) << "\",\n";
    os << "    \"view\": \"" << to_string(pi.view()) << "\",\n";
    if (pi.hasEnumLimit())
        os << "    \"enum\": " << to_string2(pi.enumValues()) << ",\n";
    if (pi.isFloat()) {
        if (pi.hasConstraintsMin())
            os << "    \"min\": " << pi.minFloat() << ",\n";
        if (pi.hasConstraintsMax())
            os << "    \"max\": " << pi.maxFloat() << ",\n";
    } else if (pi.isInt()) {
        if (pi.hasConstraintsMin())
            os << "    \"min\": " << pi.minInt() << ",\n";
        if (pi.hasConstraintsMax())
            os << "    \"max\": " << pi.maxInt() << ",\n";
    }

    switch (pi.type()) {
    case PropValueType::BOOLEAN:
        os << "    \"default\": " << pi.defaultBool() << ",\n";
        break;
    case PropValueType::FLOAT:
        os << "    \"default\": " << pi.defaultFloat() << ",\n";
        break;
    case PropValueType::INTEGER:
        os << "    \"default\": " << pi.defaultInt() << ",\n";
        break;
    case PropValueType::LIST:
        os << "    \"default\": " << pi.defaultList() << ",\n";
        break;
    case PropValueType::SYMBOL:
        os << "    \"default\": \"" << pi.defaultSymbol(&s_)->s_name << "\",\n";
        break;
    case PropValueType::VARIANT:
        if (!pi.defaultAtom().isNone())
            os << "    \"default\": " << pi.defaultAtom() << ",\n";
        break;
    }

    if (pi.units() != PropValueUnits::UNKNOWN)
        os << "    \"units\": \"" << to_string(pi.units()) << "\",\n";

    os << "    \"name\": \"" << pi.name()->s_name << "\",\n";
    os << "    \"access\": \"" << to_string(pi.access()) << "\",\n";
    os << "    \"visibility\": \"" << to_string(pi.visibility()) << "\"\n";
    os << "  }";
}

int main(int argc, char* argv[])
{
    pd_init();
    ceammc_init();

    sys_printtostderr = 1;

    if (argc < 2) {
        cerr << "usage: " << platform::basename(argv[0]) << " OBJECT_NAME [ARGS]" << endl;
        return 1;
    }

    CanvasPtr cnv = PureData::instance().createTopCanvas("/test_canvas");

    if (!cnv) {
        cerr << "can't create root canvas" << endl;
        return 2;
    }

    AtomList pd_args;
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            t_binbuf* b = binbuf_new();
            if (b) {
                binbuf_text(b, argv[i], strlen(argv[i]));
                auto n = binbuf_getnatom(b);
                auto v = binbuf_getvec(b);

                for (int j = 0; j < n; j++)
                    pd_args.append(Atom(v[j]));

                binbuf_free(b);
            }
        }
    }

    // stk rawwaves path
    if (getenv("RAWWAVES") != nullptr) {
        stk::Stk::setRawwavePath(getenv("RAWWAVES"));
    }

    pd::External ext(argv[1], pd_args);
    if (!ext.object()) {
        cerr << "can't create object: " << argv[1] << endl;
        return 3;
    }

    if (!ext.isCeammc()) {
        cerr << "not a CEAMMC object: " << argv[1] << endl;
        return 4;
    }

    bool first = true;
    cout << "{\n";

    for (auto& p : ext.properties()) {
        //        if (p.isInternal())
        //            continue;

        if (!first)
            cout << ",\n";

        if (first)
            first = false;

        printInfo(cout, p);
    }

    cout << "\n}\n";

    return 0;
}
