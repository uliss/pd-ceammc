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
#ifndef CEAMMC_PATH_H
#define CEAMMC_PATH_H

#include <atomic>

#include "path_async_base.h"
using namespace ceammc;

class Path : public PathAsyncBase<std::string> {
public:
    enum State {
        INIT,
        SEARCH,

    };

private:
    std::string path_;
    std::string needle_;
    State state_;
    IntProperty* max_depth_;
    IntProperty* max_entries_;
    std::atomic_bool async_quit_;

public:
    Path(const PdArgs& args);
    ~Path();

    void onBang() override;
    void onSymbol(t_symbol* s) override;

    void m_search(t_symbol* s, const AtomListView& lv);
    void m_append(t_symbol* s, const AtomListView& lv);

private:
    void processResult() override;
    Path::FutureResult createTask() override;
};

void setup_path_object();

#endif // CEAMMC_PATH_H
