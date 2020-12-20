/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "canvas_name.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"

CanvasName::CanvasName(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void CanvasName::onBang()
{
    symbolTo(0, canvas_info_name(canvas()));
}

void setup_base_canvas_name()
{
    ObjectFactory<CanvasName> obj("canvas.name");

    obj.setDescription("current canvas name");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"canvas", "name"});
    obj.setCategory("patch");
    obj.setSinceVersion(0, 8);
}
