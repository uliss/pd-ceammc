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
#include "ceammc_search.h"
#include "ceammc_factory.h"
#include "ceammc_object_info.h"

CeammcSearch::CeammcSearch(const PdArgs& args)
    : BaseObject(args)
    , max_(nullptr)
{
    createOutlet();

    max_ = new IntProperty("@max", 10);
    max_->checkClosedRange(1, 50);
    addProperty(max_);

    auto& st = ObjectInfoStorage::instance();
    std::copy(st.baseSet().begin(), st.baseSet().end(), std::back_inserter(all_classes_));
    std::copy(st.uiSet().begin(), st.uiSet().end(), std::back_inserter(all_classes_));
    std::copy(st.flextSet().begin(), st.flextSet().end(), std::back_inserter(all_classes_));
}

void CeammcSearch::onSymbol(t_symbol* s)
{
    AtomList res;
    int n = 0;

    auto& st = ObjectInfoStorage::instance();
    for (auto c : all_classes_) {
        if (st.hasInfo(c)) {
            auto& info = st.info(c);
            auto it = info.dict.find("description");
            if (it != info.dict.end() && it->second.find(s->s_name) != std::string::npos) {
                res.append(gensym(class_getname(c)));
                if (++n >= max_->value()) // check for max results
                    goto done;
            }
        }
    }

done:
    listTo(0, res);
}

void CeammcSearch::m_keywords(t_symbol* s, const AtomListView& l)
{
    if (!checkArgs(l, ARG_SYMBOL, s))
        return;

    t_symbol* subj = l.asT<t_symbol*>();
    AtomList res;
    int n = 0;

    auto& st = ObjectInfoStorage::instance();
    for (auto c : all_classes_) {
        if (st.hasInfo(c)) {
            auto& info = st.info(c);
            for (auto& kw : info.keywords) {
                if (kw.find(subj->s_name) != std::string::npos) {
                    res.append(gensym(class_getname(c)));
                    if (++n >= max_->value()) // check for max results
                        goto done;
                }
            }
        }
    }

done:
    listTo(0, res);
}

void CeammcSearch::m_objects(t_symbol* s, const AtomListView& l)
{
    if (!checkArgs(l, ARG_SYMBOL, s))
        return;

    t_symbol* subj = l.asT<t_symbol*>();
    AtomList res;
    int n = 0;

    auto& st = ObjectInfoStorage::instance();

    for (auto c : all_classes_) {
        std::string name(class_getname(c));
        if (name.find(subj->s_name) != std::string::npos) {
            res.append(gensym(name.c_str()));
            if (++n >= max_->value())
                goto done;

        } else if (st.hasInfo(c)) {
            for (auto& alias : st.info(c).aliases) {
                if (alias.find(subj->s_name) != std::string::npos) {
                    res.append(gensym(alias.c_str()));
                    if (++n >= max_->value())
                        goto done;
                }
            }
        }
    }

done:
    listTo(0, res);
}

void setup_ceammc_search()
{
    ObjectFactory<CeammcSearch> obj("ceammc.search");
    obj.addAuthor("Serge Poltavski");
    obj.setDescription("ceammc library search object");
    obj.setCategory("base");
    obj.setKeywords({ "search" });

    obj.addMethod("keywords", &CeammcSearch::m_keywords);
    obj.addMethod("k", &CeammcSearch::m_keywords);
    obj.addMethod("objects", &CeammcSearch::m_objects);
    obj.addMethod("o", &CeammcSearch::m_objects);
}
