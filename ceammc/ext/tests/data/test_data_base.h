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
#ifndef TEST_DATA_BASE_H
#define TEST_DATA_BASE_H

#include "ceammc_data.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_set.h"
#include "datatype_string.h"
#include "test_base.h"
#include "test_catch2.hpp"
#include "test_external.h"

using namespace ceammc;

using AT = Atom;
using AL = AtomList;

using SetA = SetAtom;
using DictA = DictAtom;
using StrA = StringAtom;
using MLA = MListAtom;

using IntA = DataAtom<IntData>;

#endif // TEST_DATA_BASE_H
