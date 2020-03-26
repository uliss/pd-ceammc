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
#ifndef ARG_GROUP_H
#define ARG_GROUP_H

#include "arg_checker_node.h"

#include <vector>

namespace ceammc {

class ArgGroup : public ArgCheckerNode {
public:
    ArgGroup() = default;

    bool insertChild(const ArgCheckerNodePtr& node) override;
    bool empty() const { return nodes_.size(); }
    size_t size() const { return nodes_.size(); }
    bool isLeaf() const final;

protected:
    std::vector<ArgCheckerNodePtr> nodes_;
};

}

#endif // ARG_GROUP_H
