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

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

static t_symbol* any = &s_anything;
extern "C" void pd_init();
t_class* ceammc_class = nullptr;

using namespace std;
using namespace ceammc;

static const char* to_string(PropertyInfoType t)
{
    switch (t) {
    case PropertyInfoType::BOOLEAN:
        return "bool";
    case PropertyInfoType::FLOAT:
        return "float";
    case PropertyInfoType::INTEGER:
        return "int";
    case PropertyInfoType::LIST:
        return "list";
    case PropertyInfoType::SYMBOL:
        return "symbol";
    case PropertyInfoType::VARIANT:
        return "atom";
    }
}

static const char* to_string(PropertyInfoView v)
{
    switch (v) {
    case PropertyInfoView::SLIDER:
        return "slider";
    case PropertyInfoView::KNOB:
        return "knob";
    case PropertyInfoView::TOGGLE:
        return "toggle";
    case PropertyInfoView::COLOR:
        return "color";
    case PropertyInfoView::NUMBOX:
        return "numbox";
    default:
        return "entry";
    }
}

static const char* to_string(PropertyInfoUnits u)
{
    switch (u) {
    case PropertyInfoUnits::MSEC:
        return "millisecond";
    case PropertyInfoUnits::SEC:
        return "second";
    case PropertyInfoUnits::DB:
        return "decibel";
    case PropertyInfoUnits::HZ:
        return "herz";
    case PropertyInfoUnits::PERCENT:
        return "percent";
    case PropertyInfoUnits::RAD:
        return "radian";
    case PropertyInfoUnits::DEG:
        return "degree";
    case PropertyInfoUnits::SEMITONE:
        return "semitone";
    case PropertyInfoUnits::CENT:
        return "cent";
    case PropertyInfoUnits::TONE:
        return "tone";
    case PropertyInfoUnits::SAMP:
        return "sample";
    default:
        return "";
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
    os << "  \"" << pi.name() << "\": {\n";
    os << "    \"type\": \"" << to_string(pi.type()) << "\",\n";
    os << "    \"view\": \"" << to_string(pi.view()) << "\",\n";
    if (pi.hasEnumLimit())
        os << "    \"enum\": " << to_string2(pi.enumValues()) << ",\n";
    if (pi.hasMinLimit())
        os << "    \"min\": " << pi.min() << ",\n";
    if (pi.hasMaxLimit())
        os << "    \"max\": " << pi.max() << ",\n";

    switch (pi.type()) {
    case PropertyInfoType::BOOLEAN:
        os << "    \"default\": " << pi.defaultBool() << ",\n";
        break;
    case PropertyInfoType::FLOAT:
        os << "    \"default\": " << pi.defaultFloat() << ",\n";
        break;
    case PropertyInfoType::INTEGER:
        os << "    \"default\": " << pi.defaultInt() << ",\n";
        break;
    case PropertyInfoType::LIST:
        os << "    \"default\": " << pi.defaultList() << ",\n";
        break;
    case PropertyInfoType::SYMBOL:
        os << "    \"default\": \"" << pi.defaultSymbol()->s_name << "\",\n";
        break;
    case PropertyInfoType::VARIANT:
        if (!pi.defaultAtom().isNone())
            os << "    \"default\": " << pi.defaultAtom() << ",\n";
        break;
    }

    if (pi.units() != PropertyInfoUnits::UNKNOWN)
        os << "    \"units\": \"" << to_string(pi.units()) << "\",\n";

    os << "    \"name\": \"" << pi.name() << "\",\n";
    os << "    \"readonly\": " << (pi.readonly() ? 1 : 0) << "\n";
    os << "  }";
}

int main(int argc, char* argv[])
{
    pd_init();
    ceammc_init();

    if (argc < 2) {
        cerr << "usage: " << platform::basename(argv[0]) << " OBJECT_NAME" << endl;
        return 1;
    }

    CanvasPtr cnv = PureData::instance().createTopCanvas("/test_canvas");

    if (!cnv) {
        cerr << "can't create root canvas" << endl;
        return 2;
    }

    pd::External ext(argv[1], AtomList());
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
        if (!first)
            cout << ",\n";

        if (first)
            first = false;

        printInfo(cout, p);
    }

    cout << "\n}\n";

    return 0;
}
