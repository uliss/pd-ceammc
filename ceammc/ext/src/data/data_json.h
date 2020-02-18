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
#ifndef DATAJSON_H
#define DATAJSON_H

#include "ceammc_data.h"
#include "data_protocol.h"
#include "datatype_json.h"

using namespace ceammc;

class DataJson : public CollectionIFace<BaseObject> {
    DataTypeJson json_;

public:
    DataJson(const PdArgs& args);

public:
    void proto_add(const AtomList& lst) final;
    bool proto_remove(const AtomList& lst) final;
    void proto_set(const AtomList& lst) final;
    void proto_clear() final;

    size_t proto_size() const final;

    void onBang() final;
    void dump() const final;
};

void setup_data_json();

#endif // DATAJSON_H
