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
#ifndef CEAMMC_OBJECT_INFO_H
#define CEAMMC_OBJECT_INFO_H

#include "m_pd.h"

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace ceammc {

class ObjectInfoStorage {
public:
    using Dict = std::map<std::string, std::string>;
    using AuthorList = std::vector<std::string>;
    using AliasList = std::vector<std::string>;
    using Keywords = std::vector<std::string>;
    using Version = std::pair<uint8_t, uint8_t>;
    using ApiVersion = uint16_t;

    struct Info {
        Dict dict;
        AuthorList authors;
        AliasList aliases;
        Keywords keywords;
        Version since_version;
        ApiVersion api;
        bool deprecated;

        Info();
    };

    using ClassMap = std::map<t_class*, Info>;
    using ClassSet = std::set<t_class*>;

    enum SetType {
        SET_NONE = 0,
        SET_BASE,
        SET_FLEXT,
        SET_UI
    };

private:
    ObjectInfoStorage();
    ObjectInfoStorage(const ObjectInfoStorage&) = delete;
    void operator=(const ObjectInfoStorage&) = delete;

    ClassSet base_set_, flext_set_, ui_set_;
    ClassMap class_map_;
    std::string lib_;
    Version lib_version_;

public:
    static ObjectInfoStorage& instance();

    Info& info(t_class* c) { return class_map_[c]; }
    bool hasInfo(t_class* c) const;
    bool find(t_symbol* name, Info& info) const;

    // common information
    const std::string& libName() const;
    const Version& libVersion() const;
    std::string libStrVersion() const;

    // sets
    const ClassSet& baseSet() const { return base_set_; }
    const ClassSet& flextSet() const { return flext_set_; }
    const ClassSet& uiSet() const { return ui_set_; }

    bool findInBase(t_class* c) const { return base_set_.find(c) != base_set_.end(); }
    bool findInFlext(t_class* c) const { return flext_set_.find(c) != flext_set_.end(); }
    bool findInUI(t_class* c) const { return ui_set_.find(c) != ui_set_.end(); }

    SetType findInSets(t_class* c) const
    {
        if (findInBase(c))
            return SET_BASE;
        else if (findInFlext(c))
            return SET_FLEXT;
        else if (findInUI(c))
            return SET_UI;
        else
            return SET_NONE;
    }

    void addBase(t_class* c, t_newmethod creator);
    void addFlext(t_class* c);
    void addUI(t_class* c);

    static void addAlias(const char* name, t_class* c, t_newmethod creator);
};

}

#endif // CEAMMC_OBJECT_INFO_H
