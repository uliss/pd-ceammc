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
#ifndef CEAMMC_RTREE_H
#define CEAMMC_RTREE_H

#include "datatype_mlist.h"

#include <iostream>

namespace ceammc {
namespace rtree {

    /**
     * Converts rhythm tree to plain list of divisions
     * @param duration - total duration, the sum of output divisions will be equal this value
     * @param ml
     * @param errPrefix - error prefix
     * @param error output stream
     * @return division list
     */
    AtomList rhythm_tree(t_float duration, const DataTypeMList* ml, const char* errPrefix = "", std::ostream& err = std::cerr);

}
}

#endif // CEAMMC_RTREE_H
