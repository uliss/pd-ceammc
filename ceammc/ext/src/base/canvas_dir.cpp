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
#include "canvas_dir.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"

CanvasDir::CanvasDir(const PdArgs& args)
    : BaseObject(args)
{
    abs_ = new BoolProperty("@abs", true);
    addProperty(abs_);

    createOutlet();
    createOutlet();
}

void CanvasDir::onBang()
{
    int level = 0;
    auto dir = canvas_info_dir(canvas_root(canvas(), level, abs_->value()));
    floatTo(1, level);
    symbolTo(0, dir);
}

void setup_base_canvas_dir()
{
    ObjectFactory<CanvasDir> obj("canvas.dir");

    obj.setDescription("current canvas directory");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "canvas", "directory" });
    obj.setCategory("patch");
    obj.setSinceVersion(0, 8);

    obj.setXletsInfo({ "bang" }, { "symbol: canvas directory", "int: nested level" });
}
