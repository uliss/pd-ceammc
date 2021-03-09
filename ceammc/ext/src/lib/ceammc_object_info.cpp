/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "ceammc_object_info.h"
#include "ceammc_config.h"
#include "fmt/format.h"

extern "C" {
#include "m_imp.h"
}

#include <algorithm>

namespace ceammc {

ObjectInfoStorage::ObjectInfoStorage()
    : lib_("ceammc")
    , lib_version_({ CEAMMC_LIB_VERSION_MAJOR, CEAMMC_LIB_VERSION_MINOR })
{
}

ObjectInfoStorage& ObjectInfoStorage::instance()
{
    static ObjectInfoStorage instance_;
    return instance_;
}

bool ObjectInfoStorage::hasInfo(t_class* c) const
{
    return class_map_.find(c) != class_map_.end();
}

bool ObjectInfoStorage::find(t_symbol* name, ObjectInfoStorage::Info& info) const
{
    auto it = std::find_if(
        class_map_.begin(),
        class_map_.end(),
        [name](const ClassMap::value_type& v) { return v.first->c_name == name; });

    if (it == class_map_.end())
        return false;
    else {
        info = it->second;
        return true;
    }
}

const std::string& ObjectInfoStorage::libName() const
{
    return lib_;
}

const ObjectInfoStorage::Version& ObjectInfoStorage::libVersion() const
{
    return lib_version_;
}

std::string ObjectInfoStorage::libStrVersion() const
{
    return fmt::format("{}.{}", (int)lib_version_.first, (int)lib_version_.second);
}

void ObjectInfoStorage::addBase(t_class* c, t_newmethod creator)
{
    base_set_.insert(c);

    // check for global name - add alias if needed
    std::string name(c->c_name->s_name);
    if (name.find('.') == std::string::npos) {
        std::string ceammc_alias("ceammc/");
        ceammc_alias += name;

        class_addcreator(creator, gensym(ceammc_alias.c_str()), A_GIMME, A_NULL);
        instance().info(c).aliases.push_back(ceammc_alias);
    }
}

void ObjectInfoStorage::addFlext(t_class* c)
{
    flext_set_.insert(c);
}

void ObjectInfoStorage::addUI(t_class* c)
{
    ui_set_.insert(c);
}

void ObjectInfoStorage::addAlias(const char* name, t_class* c, t_newmethod creator)
{
    std::string alias(name);
    // find global like alias
    if (alias.find('.') == std::string::npos) {
        std::string ceammc_alias("ceammc/");
        ceammc_alias += alias;

        class_addcreator(creator, gensym(ceammc_alias.c_str()), A_GIMME, A_NULL);
        instance().info(c).aliases.push_back(ceammc_alias);
    }

    class_addcreator(creator, gensym(name), A_GIMME, A_NULL);
    instance().info(c).aliases.push_back(name);
}

ObjectInfoStorage::Info::Info()
    : since_version({ 0, 1 })
    , api(0)
    , deprecated(false)
{
}

}
