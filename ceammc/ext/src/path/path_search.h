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
#ifndef PATH_SEARCH_H
#define PATH_SEARCH_H

#include "ceammc_data.h"
#include "path_async_base.h"
using namespace ceammc;

class PathSearch : public PathAsyncBase<std::string> {
    ListProperty* paths_;
    IntProperty* depth_;
    BoolProperty* home_;
    BoolProperty* std_;
    std::atomic_bool search_stop_;
    std::string needle_;

public:
    PathSearch(const PdArgs& args);
    ~PathSearch();

    void onSymbol(t_symbol* s) override;
    void onDataT(const StringAtom& a);

    void m_cancel(t_symbol* s, const AtomListView& lv);

private:
    void processResult() override;
    FutureResult createTask() override;
};

void setup_path_search();

#endif // PATH_SEARCH_H
