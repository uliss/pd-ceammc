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
#ifndef DICT_VALUES_H
#define DICT_VALUES_H

#include "datatype_dict.h"
#include "dict_base.h"

using namespace ceammc;

class DictValues : public DictBase {
public:
    DictValues(const PdArgs& args);
    void onDataT(const DictAtom& dict) override;
};

void setup_dict_values();

#endif // DICT_VALUES_H
