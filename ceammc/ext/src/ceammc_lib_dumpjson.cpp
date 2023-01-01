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
#include "ceammc_object_info.h"
#include "ceammc_pd.h"
#include "datatype_dict.h"
#include "stk/stk/include/Stk.h"
#include "json/json.hpp"

extern "C" {
#include "m_imp.h"
}

using json = nlohmann::json;

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#ifndef STK_RAWWAVES
#define STK_RAWWAVES "stk/rawwaves"
#endif

using namespace ceammc;

extern "C" CEAMMC_EXTERN bool ceammc_dump_json(int argc, char* argv[])
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

    json j;
    json jobj;

    jobj["name"] = pd::object_name(ext.object())->s_name;
    jobj["dir"] = pd::object_dir(ext.object())->s_name;

    jobj["inlets"] = json::array();
    for (int i = 0; i < ext.numInlets(); i++) {
        if (ext.inletInfo(i).isSignal())
            jobj["inlets"].push_back("audio");
        else
            jobj["inlets"].push_back("control");
    }

    jobj["outlets"] = json::array();
    for (int i = 0; i < ext.numOutlets(); i++) {
        if (ext.outletInfo(i).isSignal())
            jobj["outlets"].push_back("audio");
        else
            jobj["outlets"].push_back("control");
    }

    // add methods
    for (auto m : ext.methods()) {
        const char* name = m->s_name;
        if (name[0] == '@' || name[0] == '.')
            continue;

        jobj["methods"].push_back(name);
    }

    // arguments
    std::vector<PropertyInfo> obj_args;

    // add properties
    for (const PropertyInfo& i : ext.properties()) {
        DataTypeDict info;
        if (!i.getDict(info)) {
            std::cerr << "can't get property info: " << i.name()->s_name << std::endl;
            continue;
        }

        jobj["properties"].push_back(json::parse(info.toJsonString()));

        if (i.hasArgIndex()) {
            obj_args.insert(std::upper_bound(obj_args.begin(), obj_args.end(), i,
                                [](const PropertyInfo& a, const PropertyInfo& b) { return a.argIndex() < b.argIndex(); }),
                i);
        }
    }

    // output args
    if (obj_args.size() > 0) {
        for (auto& arg_info : obj_args) {
            auto name = boost::to_upper_copy(std::string(arg_info.name()->s_name + 1));
            jobj["args"].push_back({
                { "name", name },
                { "property", arg_info.name()->s_name },
                { "index", arg_info.argIndex() },
                { "type", to_symbol(arg_info.type())->s_name },
            });
        }
    }

    // add info
    t_class* obj_class = pd::object_class(ext.object());
    if (ObjectInfoStorage::instance().hasInfo(obj_class)) {
        auto info = ObjectInfoStorage::instance().info(obj_class);
        auto jinfo = json::object();

        jinfo["deprecated"] = info.deprecated;
        jinfo["keywords"] = info.keywords;
        jinfo["authors"] = info.authors;
        jinfo["aliases"] = info.aliases;
        jinfo["api"] = info.api;
        jinfo["since"] = info.since_version;

        for (auto& kv : info.dict)
            jinfo[kv.first] = kv.second;

        jobj["info"] = jinfo;
    }

    j["object"] = jobj;

    std::cout << j.dump(2);

    return true;
}
