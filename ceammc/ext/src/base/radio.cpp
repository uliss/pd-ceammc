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
#include "radio.h"
#include "ceammc_factory.h"

constexpr int DEFAULT_OUTLETS = 2;
constexpr int MIN_OUTLETS = 2;
constexpr int MAX_OUTLETS = 24;

Radio::Radio(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
{
    n_ = new IntProperty("@n", DEFAULT_OUTLETS);
    n_->setArgIndex(0);
    n_->checkClosedRange(MIN_OUTLETS, MAX_OUTLETS);
    addProperty(n_);

    parseProperties();

    for (int i = 0; i < n_->value(); i++)
        createOutlet();
}

void Radio::onFloat(t_float f)
{
    int idx = f;

    if (idx < 0 || idx >= n_->value()) {
        OBJ_ERR << "invalid radio index: " << f;
        return;
    }

    for (int i = n_->value(); i > 0; i--) {
        int el_idx = i - 1;
        floatTo(el_idx, idx == el_idx ? 1 : 0);
    }
}

void Radio::m_reset(t_symbol* s, const AtomListView&)
{
    for (size_t i = n_->value(); i > 0; i--)
        floatTo(i - 1, 0);
}

void setup_base_radio()
{
    ObjectFactory<Radio> obj("radio");
    obj.addMethod("reset", &Radio::m_reset);

    obj.setDescription("float index to outlet values as radio switch");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "radio" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 6);
}
