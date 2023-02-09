/*****************************************************************************
 * Copyright 2022 Serge Poltavski. All rights reserved.
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
#include "array_circular.h"
#include "ceammc_factory.h"

ArrayCircular::ArrayCircular(const PdArgs& args)
    : ArrayMod(args)
{
    createOutlet();
}

void ArrayCircular::onFloat(t_float f)
{
    if (!checkArray(true))
        return;

    try {
        floatTo(0, array_.ringPushBack(f));

        if (shouldRedraw())
            array_.redraw();
    } catch (std::exception& e) {
        OBJ_ERR << "can't push value: " << e.what();
    }
}

void setup_array_circular()
{
    ObjectFactory<ArrayCircular> obj("array.circular");

    obj.setDescription("array read/write as circular buffer");
    obj.setCategory("array");
    obj.setKeywords({"array", "circular"});
}
