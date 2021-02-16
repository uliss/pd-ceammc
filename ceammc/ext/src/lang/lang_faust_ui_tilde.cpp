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

    addProperty(new FloatProperty("@a", 100))->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 50, 200);
    addProperty(new FloatProperty("@b", 100))->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 50, 200);
    addProperty(new FloatProperty("@c", -100))->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 50, 200);

    property("@a")->setSuccessFn([this](Property*) {
        notifyPropUpdate(0);
    });

    property("@b")->setSuccessFn([this](Property*) {
        notifyPropUpdate(1);
    });

    createInlet();

    buildUI();
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
    auto sz = view_.build(properties());
    setSize(sz.width(), sz.height());
    LIB_ERR << size();
}

void setup_lang_faust_ui_tilde()
{
    static const bool init = tcl_nui_init();

    UISoundFactory<LangFaustUiTilde> obj("ui");
}

static t_symbol* genid(void* x)
{
    char buf[64];
    snprintf(buf, 63, "#%lx", x);
    return gensym(buf);
}

WidgetIFace::WidgetIFace(t_object* x, t_glist* widget_parent)
    : x_(x)
    , widget_parent_(widget_parent)
    , widget_canvas_(canvas_getrootfor(widget_parent_))
    , size_(0, 0)
    , event_proxy_(this, genid(&event_proxy_))
    , view_(widget_parent)
{
}

WidgetIFace::~WidgetIFace()
{
}

Rect WidgetIFace::getRealRect(t_glist* window) const
{
    return Rect(text_xpix(x_, window), text_ypix(x_, window), size_);
}

Rect WidgetIFace::getRect(t_glist* window) const
{
    auto z = window->gl_zoom;
    Rect res(text_xpix(x_, window), text_ypix(x_, window), size_ * z);
    //    LIB_ERR << __FUNCTION__ << res;
    return res;
}

void WidgetIFace::displaceWidget(t_glist* window, int dx, int dy)
{
    x_->te_xpix += dx;
    x_->te_ypix += dy;

    LIB_ERR << __FUNCTION__;

    if (glist_isvisible(widget_parent_)) {
        LIB_ERR << __FUNCTION__;
        PointF pos(text_xpix(x_, window), text_ypix(x_, window));
        const float rz = 1.0 / glist_getzoom(window);
        view_.move(pos * rz);
        canvas_fixlinesfor(window, x_);
    }
}

void WidgetIFace::deleteWidget(t_glist* window)
{
    LIB_ERR << __FUNCTION__;
    //    for (auto& v : view_list_)
    //        v->erase(window);

    canvas_deletelinesfor(window, x_);
}

void WidgetIFace::selectWidget(t_glist* window, bool state)
{
    view_.select(state);
}

void WidgetIFace::showWidget(t_glist* window)
{
    LIB_ERR << __FUNCTION__;
    view_.create(reinterpret_cast<WinId>(window),
        getRect(window).pt0(),
        glist_getzoom(window));
}

void WidgetIFace::hideWidget(t_glist* window)
{
    LIB_ERR << __FUNCTION__;
    view_.erase();

    sys_unqueuegui(x_);
}

bool WidgetIFace::visible() const
{
    return glist_isvisible(widget_parent_);
}

void WidgetIFace::setSize(int w, int h)
{
    size_ = Size(w, h);
}

void WidgetIFace::notifyPropUpdate(int idx)
{
    LIB_ERR << __FUNCTION__ << " prop changed: " << idx;
    view_.update(idx);
}

void WidgetIFace::bindEvents()
{
    sys_vgui("::ceammc::ui::mouse_events_bind .%lx.c.w #%lx"
             "down up move enter leave right_click\n",
        widget_parent_, &event_proxy_);
}

void MouseEvents::onMouseEnter()
{
    dest_->onMouseEnter();
}

void FaustMasterView::create(WinId win, const PointF& pos, float zoom)
{
    int z = 1;
    Size subcnv_size = vframe_.size() * zoom;
    Point p = pos;

    sys_vgui("nui::widget_create %lx %lx %d %d %d %d %d\n",
        win, this, p.x(), p.y(), subcnv_size.width(), subcnv_size.height(), z);

    vframe_.create(win, zoom);
}

void FaustMasterView::erase()
{
    vframe_.erase();
    sys_vgui("nui::widget_erase %lx %lx\n", parent_, this);
}

void FaustMasterView::move(const PointF& pos)
{
    Point p = pos;

    sys_vgui("nui::widget_move %lx %lx %d %d\n", parent_, this, p.x(), p.y());
}

void FaustMasterView::layout()
{
    vframe_.layout();
}

Size FaustMasterView::build(const std::vector<Property*>& props)
{
    for (auto* p : props)
        addProperty(p);

    layout();
    LIB_ERR << vframe_.size();
    return vframe_.size();
}
