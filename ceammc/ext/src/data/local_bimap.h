/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef LOCAL_BIMAP_H
#define LOCAL_BIMAP_H

#include "data_bimap.h"
#include "local_base.h"

using LocalBiMapBase = LocalBase<std::pair<BiMap, AtomList>, DataBiMapBase>;

class LocalBiMap : public LocalBiMapBase {
public:
    LocalBiMap(const PdArgs& args);
    BiMap& bimap() final { return ref().first; }
    const BiMap& bimap() const final { return ref().first; }
    AtomList& list() final { return ref().second; }
    const AtomList& list() const final { return ref().second; }
};

void setup_local_bimap();

#endif // LOCAL_BIMAP_H
