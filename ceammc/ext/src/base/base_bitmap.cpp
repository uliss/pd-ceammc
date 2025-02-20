/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#include "base_bitmap.h"
#include "ceammc_factory.h"

using namespace ceammc;

BaseBitmap::BaseBitmap(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    w_ = new IntProperty("@w", 8);
    w_->checkClosedRange(1, 256);
    w_->setArgIndex(0);
    addProperty(w_);

    h_ = new IntProperty("@h", 8);
    h_->checkClosedRange(1, 256);
    h_->setArgIndex(1);
    addProperty(h_);

    createOutlet();
}

BaseBitmap::~BaseBitmap()
{
    ceammc_bitmap_free(bm_);
}

void BaseBitmap::initDone()
{
    bm_ = ceammc_bitmap_new(w_->value(), h_->value(),
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } },
        { this, [](void* user, const std::uint8_t* data, size_t len) {
             auto obj = static_cast<BaseBitmap*>(user);
             if (!obj)
                 return;

             AtomList res;
             res.reserve(len);
             for (size_t i = 0; i < len; i++) {
                 res.push_back(data[i]);
             }

             obj->listTo(0, res);
         } },
        { this, [](void* user, const char* msg) {
             auto obj = static_cast<BaseBitmap*>(user);
             if (!obj)
                 return;

             Error(obj) << msg;
         } });
}

bool BaseBitmap::notify(int code)
{
    ceammc_bitmap_process(bm_);
    return true;
}

void BaseBitmap::onBang()
{
    ceammc_bitmap_get(bm_);
}

void BaseBitmap::m_clear(t_symbol* s, const AtomListView& lv)
{
    ceammc_bitmap_clear(bm_);
}

void BaseBitmap::m_text(t_symbol* s, const AtomListView& lv)
{
    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    ceammc_bitmap_draw_text(bm_, lv.symbolAt(2, &s_)->s_name, x, y);
}

void BaseBitmap::m_fill(t_symbol* s, const AtomListView& lv)
{
    ceammc_bitmap_fill(bm_, lv.boolAt(0, false));
}

void BaseBitmap::m_invert(t_symbol* s, const AtomListView& lv)
{
    ceammc_bitmap_invert(bm_);
}

void setup_base_bitmap()
{
    ObjectFactory<BaseBitmap> obj("bitmap");

    obj.addMethod("text", &BaseBitmap::m_text);
    obj.addMethod("clear", &BaseBitmap::m_clear);
    obj.addMethod("fill", &BaseBitmap::m_fill);
    obj.addMethod("invert", &BaseBitmap::m_invert);
}
