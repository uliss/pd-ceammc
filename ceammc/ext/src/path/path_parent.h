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
#ifndef PATH_PARENT_H
#define PATH_PARENT_H

#include "ceammc_data.h"
#include "ceammc_object.h"
using namespace ceammc;

class PathParent : public BaseObject {
public:
    PathParent(const PdArgs& args);

    void onSymbol(t_symbol* s) override;
    void onDataT(const StringAtom& s);
};

void setup_path_parent();

#endif // PATH_PARENT_H
