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
#include "ceammc.h"
#include "ceammc_atomlist.h"
#include "ceammc_format.h"
#include "ceammc_object_info.h"
#include "ceammc_pd.h"
#include "datatype_dict.h"
#include "stk/stk/include/Stk.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#ifndef STK_RAWWAVES
#define STK_RAWWAVES "stk/rawwaves"
#endif

using namespace ceammc;

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

static std::string pddoc_units(const std::string& u)
{
    const static std::map<std::string, std::string> um = {
        { "samp", "sample" },
        { "db", "decibel" },
        { "hz", "herz" },
        { "msec", "millisecond" },
        { "sec", "second" },
        { "rad", "radian" },
        { "deg", "degree" }
    };
    auto it = um.find(u);
    if (it == um.end())
        return u;
    else
        return it->second;
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
        os << "    \"default\": " << to_json_string(pi.defaultList()) << ",\n";
        break;
    case PropValueType::SYMBOL:
        os << "    \"default\": " << to_json_string(pi.defaultSymbol(&s_)) << ",\n";
        break;
    case PropValueType::ATOM:
        if (!pi.defaultAtom().isNone())
            os << "    \"default\": " << to_json_string(pi.defaultAtom()) << ",\n";
        break;
    }

    if (pi.units() != PropValueUnits::NONE)
        os << "    \"units\": \"" << pddoc_units(to_string(pi.units())) << "\",\n";

    os << "    \"name\": \"" << pi.name()->s_name << "\",\n";
    os << "    \"access\": \"" << to_string(pi.access()) << "\",\n";
    os << "    \"visibility\": \"" << to_string(pi.visibility()) << "\"\n";
    os << "  }";
}

extern "C" CEAMMC_EXTERN bool ceammc_list_props(int argc, char* argv[])
{
    CanvasPtr cnv = PureData::instance().createTopCanvas("/test_canvas");

    if (!cnv) {
        std::cerr << "can't create root canvas" << std::endl;
        return false;
    }

    AtomList args;
    if (argc > 2) {
        t_binbuf* b = binbuf_new();

        std::string str;
        for (int i = 2; i < argc; i++) {
            str += argv[i];
            str += " ";
        }

        binbuf_text(b, str.c_str(), str.size());

        int n = binbuf_getnatom(b);

        for (int i = 0; i < n; i++)
            args.append(binbuf_getvec(b)[i]);

        binbuf_free(b);
    }

    // stk rawwaves path
    if (getenv("RAWWAVES") != nullptr)
        stk::Stk::setRawwavePath(getenv("RAWWAVES"));

    pd::External ext(argv[0], args);
    if (!ext.object()) {
        std::cerr << "can't create object: " << argv[0] << std::endl;
        return false;
    }

    if (!ext.isCeammc()) {
        std::cerr << "not a CEAMMC object: " << argv[0] << std::endl;
        return false;
    }

    bool first = true;
    std::cout << "{\n";

    for (auto& p : ext.properties()) {
        //        if (p.isInternal())
        //            continue;

        if (!first)
            std::cout << ",\n";

        if (first)
            first = false;

        printInfo(std::cout, p);
    }

    std::cout << "\n}\n";

    return true;
}
