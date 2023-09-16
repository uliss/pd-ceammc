/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_NODE_EXTERNAL_H
#define CEAMMC_NODE_EXTERNAL_H

#include "ceammc_object.h"

#include <functional>

namespace ceammc {

using ObjectList = std::vector<t_object*>;
using PdObjectCb = std::function<bool(t_object*)>;
using BaseObjectCb = std::function<bool(const BaseObject*)>;

class NodeExternal : public BaseObject {
public:
    NodeExternal(const PdArgs& args);

    ObjectList connectedObjects() const;

    void forEach(PdObjectCb cb);
    void forEach(BaseObjectCb cb);
};

}

#endif // CEAMMC_NODE_EXTERNAL_H
