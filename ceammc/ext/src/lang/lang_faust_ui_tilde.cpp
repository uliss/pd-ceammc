/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "lang_faust_ui_tilde.h"
#include "ceammc_factory.h"

extern "C" {
#include "g_canvas.h"
}

template <typename T>
class UISoundFactory : public SoundExternalFactory<T> {
    static t_widgetbehavior wb_;

public:
    UISoundFactory(const char* name,
        int flags = OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_MAIN_SIGNAL_INLET | OBJECT_FACTORY_NO_FLOAT)
        : SoundExternalFactory<T>(name, flags)
    {
        wb_.w_getrectfn = getRect;
        wb_.w_displacefn = wdisplace;
        //        wb_.w_selectfn = iemgui_select;
        wb_.w_activatefn = nullptr;
        wb_.w_deletefn = wdelete;
        //        wb_.w_visfn = iemgui_vis;
        //        wb_.w_clickfn = bng_newclick;
        class_setwidget(this->classPointer(), &wb_);
        //        class_setsavefn(bng_class, bng_save);
        //        class_setpropertiesfn(bng_class, bng_properties);
    }

    /* call this to get a gobj's bounding rectangle in pixels */
    static void getRect(t_gobj* x, t_glist* cnv, int* x1, int* y1, int* x2, int* y2)
    {
        auto z = reinterpret_cast<typename SoundExternalFactory<T>::ObjectProxy*>(x);
        auto r = z->impl->getRect(cnv);
        *x1 = r.left();
        *y1 = r.top();
        *x2 = r.right();
        *y2 = r.bottom();
    }

    static void wdisplace(t_gobj* x, t_glist* cnv, int dx, int dy)
    {
        auto z = reinterpret_cast<typename SoundExternalFactory<T>::ObjectProxy*>(x);
        z->impl->displace(cnv, dx, dy);
    }

    static void wdelete(t_gobj* x, t_glist* cnv)
    {
        auto z = reinterpret_cast<typename SoundExternalFactory<T>::ObjectProxy*>(x);
        z->impl->deleteWidget(cnv);
    }
};

template <typename T>
t_widgetbehavior UISoundFactory<T>::wb_;

LangFaustUiTilde::LangFaustUiTilde(const PdArgs& args)
    : SoundExternal(args)
    , WidgetIFace(this->owner(), canvas())
{
}

void LangFaustUiTilde::processBlock(const t_sample** in, t_sample** out)
{
}

void setup_lang_faust_ui_tilde()
{
    UISoundFactory<LangFaustUiTilde> obj("lang.faust_ui~");
}

WidgetIFace::WidgetIFace(t_object* x, t_glist* widget_canvas)
    : x_(x)
    , widget_canvas_(widget_canvas)
    , size_(0, 0)
{
}

Rect<int> WidgetIFace::getRealRect(_glist* cnv) const
{
    return Rect<int>(text_xpix(x_, cnv), text_ypix(x_, cnv), size_);
}

Rect<int> WidgetIFace::getRect(t_glist* cnv) const
{
    auto z = cnv->gl_zoom;
    return Rect<int>(text_xpix(x_, cnv), text_ypix(x_, cnv), size_ * z);
}

void WidgetIFace::displace(t_glist* cnv, int dx, int dy)
{
    x_->te_xpix += dx;
    x_->te_ypix += dy;

    if (glist_isvisible(widget_canvas_)) {
        drawMove(cnv);
        canvas_fixlinesfor(cnv, x_);
    }
}

void WidgetIFace::deleteWidget(t_glist* cnv)
{
    canvas_deletelinesfor(cnv, x_);
}

void WidgetIFace::drawMove(t_glist* cnv)
{
}
