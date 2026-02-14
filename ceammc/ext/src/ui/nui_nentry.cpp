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
#include "nui_nentry.h"
#include "ceammc_convert.h"
#include "nui/nentry_view.h"
#include "nui/widget_factory.h"

using namespace ceammc;
using namespace ceammc::ui;

NUINumEntryBase::NUINumEntryBase(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

NUINumEntry::NUINumEntry(const PdArgs& args)
    : SimpleTclWidget<NUINumEntryBase>(args)
{
    using sc = StyleCollection;
    auto sz = sc::size(0, "nentry:size"_hash, Size(100, 20));

    ViewPtr view(new NumEntryView(&model_, NumEntryView::ViewImplPtr(new TclNumEntryImpl()), {}));
    view->setSize(sz);
    setModelView(std::move(view));

    setSize(sz);
    setResizeMode(RESIZE_WIDTH);
}

void NUINumEntry::onBang()
{
    floatTo(0, model_.data().value());
}

void NUINumEntry::onFloat(t_float f)
{
    set(f);
    onBang();
}

void NUINumEntry::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    auto cnv = drawCanvas();

    if (cnv->gl_editor) {
        glist_grab(cnv, asGObj(), nullptr, nullptr, pt.x(), pt.y());
        cnv->gl_editor->e_textedfor = nullptr;
        cnv->gl_editor->e_textdirty = 0;
    }

    boxView().acceptEvent(EVENT_MOUSE_DOWN, pt, {});
}

void NUINumEntry::set(t_float f)
{
    model_.data().valueRef() = clip<t_float>(f, model_.data().min(), model_.data().max());
    model_.notify();
}

void setup_nui_nentry()
{
    using namespace ceammc;
    using namespace ceammc::ui;

    WidgetFactory<NUINumEntry> obj("nui.numbox");
    obj.addAlias("nnbx");
}
