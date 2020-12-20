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
#ifndef CEAMMC_SEARCH_H
#define CEAMMC_SEARCH_H

#include "ceammc_object.h"
using namespace ceammc;

class CeammcSearch : public BaseObject {
    IntProperty* max_;
    std::vector<t_class*> all_classes_;

public:
    CeammcSearch(const PdArgs& args);
    void onSymbol(t_symbol* s) override;
    void m_keywords(t_symbol* s, const AtomListView& l);
    void m_objects(t_symbol* s, const AtomListView& l);
};

void setup_ceammc_search();

#endif // CEAMMC_SEARCH_H
