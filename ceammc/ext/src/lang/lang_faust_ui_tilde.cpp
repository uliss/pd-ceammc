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
        wb_.w_selectfn = wselect;
        wb_.w_activatefn = nullptr;
        wb_.w_deletefn = wdelete;
        wb_.w_visfn = wvis;
        //        wb_.w_clickfn = bng_newclick;
        class_setwidget(this->classPointer(), &wb_);
        //        class_setsavefn(bng_class, bng_save);
        //        class_setpropertiesfn(bng_class, bng_properties);
    }

    /* call this to get a gobj's bounding rectangle in pixels */
    static void getRect(t_gobj* x, t_glist* window, int* x1, int* y1, int* x2, int* y2)
    {
        auto z = reinterpret_cast<typename SoundExternalFactory<T>::ObjectProxy*>(x);
        auto r = z->impl->getRect(window);
        *x1 = r.left();
        *y1 = r.top();
        *x2 = r.right();
        *y2 = r.bottom();
    }

    static void wdisplace(t_gobj* x, t_glist* window, int dx, int dy)
    {
        auto proxy = reinterpret_cast<typename SoundExternalFactory<T>::ObjectProxy*>(x);
        proxy->impl->displaceWidget(window, dx, dy);
    }

    static void wselect(t_gobj* x, t_glist* window, int state)
    {
        auto proxy = reinterpret_cast<typename SoundExternalFactory<T>::ObjectProxy*>(x);
        proxy->impl->selectWidget(window, state);
    }

    static void wdelete(t_gobj* x, t_glist* window)
    {
        auto proxy = reinterpret_cast<typename SoundExternalFactory<T>::ObjectProxy*>(x);
        proxy->impl->deleteWidget(window);
    }

    static void wvis(t_gobj* x, t_glist* window, int flag)
    {
        auto proxy = reinterpret_cast<typename SoundExternalFactory<T>::ObjectProxy*>(x);

        if (flag)
            proxy->impl->showWidget(window);
        else
            proxy->impl->hideWidget(window);
    }
};

template <typename T>
t_widgetbehavior UISoundFactory<T>::wb_;

LangFaustUiTilde::LangFaustUiTilde(const PdArgs& args)
    : SoundExternal(args)
    , WidgetIFace(this->owner(), canvas())
{
    setSize(100, 50);
}

LangFaustUiTilde::~LangFaustUiTilde()
{
}

void LangFaustUiTilde::processBlock(const t_sample** in, t_sample** out)
{
}

size_t LangFaustUiTilde::widgetPropCount() const
{
    return properties().size();
}

void LangFaustUiTilde::widgetPropNames(t_symbol** dest) const
{
    for (auto p : properties()) {
        auto psym = dest++;
        *psym = p->name();
    }
}

void LangFaustUiTilde::buildUI()
{
    for (auto* p : properties()) {
        view_.addProperty(p);
    }
}

void setup_lang_faust_ui_tilde()
{
    UISoundFactory<LangFaustUiTilde> obj("lang.faust_ui~");
}

WidgetIFace::WidgetIFace(t_object* x, t_glist* widget_parent)
    : x_(x)
    , widget_parent_(widget_parent)
    , widget_canvas_(canvas_getrootfor(widget_parent_))
    , size_(0, 0)
{
}

WidgetIFace::~WidgetIFace()
{
}

Rect<int> WidgetIFace::getRealRect(t_glist* window) const
{
    return Rect<int>(text_xpix(x_, window), text_ypix(x_, window), size_);
}

Rect<int> WidgetIFace::getRect(t_glist* window) const
{
    auto z = window->gl_zoom;
    return Rect<int>(text_xpix(x_, window), text_ypix(x_, window), size_ * z);
}

void WidgetIFace::displaceWidget(t_glist* window, int dx, int dy)
{
    x_->te_xpix += dx;
    x_->te_ypix += dy;

    if (glist_isvisible(widget_parent_)) {
        //        for (auto& v : view_list_)
        //            v->move(window, x_->te_xpix, x_->te_ypix);

        canvas_fixlinesfor(window, x_);
    }
}

void WidgetIFace::deleteWidget(t_glist* window)
{
    //    for (auto& v : view_list_)
    //        v->erase(window);

    canvas_deletelinesfor(window, x_);
}

void WidgetIFace::selectWidget(t_glist* window, bool state)
{
    //    for (auto& v : view_list_)
    //        v->select(window, state);
}

void WidgetIFace::showWidget(t_glist* window)
{
    //    for (auto& v : view_list_)
    //        v->create(window);
}

void WidgetIFace::hideWidget(t_glist* window)
{

    //    for (auto& v : view_list_)
    //        v->erase(window);

    sys_unqueuegui(x_);
}

bool WidgetIFace::visible() const
{
    return glist_isvisible(widget_parent_);
}

void WidgetIFace::setSize(int w, int h)
{
    size_ = Size<int>(w, h);
    //    if(visible())
    //        upda
}

//void TclCanvasSurface::clear(uint64_t obj_id)
//{
//    sys_vgui(".x%x erase #%x\n", window(), obj_id);
//}

//void TclCanvasSurface::move(uint64_t obj_id, float x, float y)
//{
//    sys_vgui(".x%x move %f %f #%x\n", window(), obj_id, x, y);
//}

//void TclCanvasSurface::rect(uint64_t obj_id, const Rect<float>& r)
//{
//    sys_vgui(".x%x create rectangle %d %d %d %d -tags {#%x}\n",
//        window(),
//        (int)r.left(), (int)r.top(), (int)r.right(), (int)r.bottom(),
//        obj_id);
//}

//void TclRemote::move(uint64_t win_id, float x, float y)
//{
//    sys_vgui(".x%lx.c moveto #%lx %d %d\n", win_id, objectId(), (int)x, (int)y);
//    //    sys_vgui("%s_move .x%lx #%lx %d %d\n", prefix(), win_id, objectId(), (int)x, (int)y);
//}

//void TclRemote::update(uint64_t win_id)
//{
//    sys_vgui("%s_update .x%lx #%lx\n", prefix(), win_id, objectId());
//}

//void TclRemote::erase(uint64_t win_id)
//{
//    sys_vgui(".x%lx.c delete #%lx\n", win_id, objectId());
//    //    sys_vgui("%s_erase .x%lx #%lx\n", prefix(), win_id, objectId());
//}

//void TclRemote::select(uint64_t win_id, bool state)
//{
//    if (state)
//        sys_vgui(".x%lx.c itemconfig #%lx -outline blue\n", win_id, objectId());
//    else
//        sys_vgui(".x%lx.c itemconfig #%lx -outline grey\n", win_id, objectId());

//    //    sys_vgui("%s_select .x%lx #%lx %d\n", prefix(), win_id, objectId(), (int)state);
//}

//void TclRemote::create(uint64_t win_id, const Rect<float>& pos)
//{
//    sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill black -outline grey -width 2 -tags #%lx\n",
//        win_id,
//        (int)pos.left(), (int)pos.top(), (int)pos.right(), (int)pos.bottom(),
//        objectId());

//    //    sys_vgui("%s_create .x%lx #%lx %d\n", prefix(), win_id, objectId());
//}
