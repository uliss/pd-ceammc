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
{
    createOutlet();
}

void CeammcSearch::onSymbol(t_symbol* s)
{
}

void CeammcSearch::m_keywords(t_symbol* s, const AtomList& l)
{
    if (!checkArgs(l, ARG_SYMBOL, s))
        return;

    t_symbol* subj = l.asT<t_symbol*>();
    AtomList res;

    auto& st = ObjectInfoStorage::instance();
    for (auto& c : st.baseSet()) {
        if (st.hasInfo(c)) {
            auto& info = st.info(c);
            for (auto& kw : info.keywords) {
                if (kw == subj->s_name) {
                    res.append(gensym(class_getname(c)));
                    goto kw_found;
                }
            }

        kw_found:;
        }
    }

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
}
