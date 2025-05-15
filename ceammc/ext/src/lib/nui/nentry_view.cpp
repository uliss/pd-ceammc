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
#include "nentry_view.h"
#include "ceammc_log.h"
#include "fmt/core.h"
#include "nui_nentry_tcl.h"

using namespace ceammc::ui;

NumEntryView::NumEntryView(NumentryModel* model, NumEntryView::ViewImplPtr&& impl, const PointF& pos)
    : ModelView<NumentryData>(model, std::move(impl), pos)
{
}

EventAcceptStatus NumEntryView::onEvent(EventType t, const PointF& pos, const EventContext& ctx)
{
    switch (t) {
    // case EVENT_MOUSE_DOWN:
    //     data().setIsOpen(!data().isOpen());
    //     redraw();
    //     notifyOthers();
    //     return { nullptr, EVENT_STATUS_ACCEPT };
    default:
        return { nullptr, EVENT_STATUS_IGNORE };
    }
}

void TclNumEntryImpl::create(const RectF& bbox, const NumentryData& data)
{
    const SizeF min(SizeF(5, 5));
    const Rect rect = transform(bbox).clippedMin(min);

    // const char* item = "{}";
    // if (data.index() >= 0 && data.index() < data.values().size())
    //     item = data.values().at(data.index())->s_name;

    // sys_vgui("nui::combobox::create %lx %lx %lx"
    //          " %d %d %d %d %d %s"
    //          " {{%s} %d}"
    //          " #%6.6x #%6.6x #%6.6x\n",
    //     winId(), widgetId(), this,
    //     rect.left(), rect.top(), rect.width(), rect.height(), (int)scale(), item,
    //     data.font().family(), int(data.font().size() * scale()),
    //     data.borderColor(), data.fillColor(), data.textColor());
}

void TclNumEntryImpl::erase()
{
    sys_vgui("nui::nentry::erase %lx %lx %lx\n", winId(), widgetId(), this);
}

void TclNumEntryImpl::update(const RectF& bbox, const NumentryData& data)
{
    const SizeF min(SizeF(5, 5));
    const Rect rect = transform(bbox).clippedMin(min);

    // std::string items;
    // for (auto& s : data.values()) {
    //     items += fmt::format(" {{{0}}}", s->s_name);
    // }

    // sys_vgui("nui::combobox::update"
    //          " %lx %lx %lx"
    //          " %d %d %d %d"
    //          " %d %s"
    //          " %d [list %s]"
    //          " {{%s} %d}"
    //          " #%6.6x #%6.6x #%6.6x #%6.6x\n",
    //     winId(), widgetId(), this,
    //     rect.left(), rect.top(), rect.width(), rect.height(),
    //     (int)scale(), data.isOpen() ? "true" : "false",
    //     data.index(), items.c_str(),
    //     data.font().family(), int(data.font().size() * scale()),
    //     data.borderColor(), data.fillColor(), data.textColor(), data.activeColor());
}

void TclNumEntryImpl::updateCoords(const RectF& bbox)
{
}

void ceammc::ui::tcl_nentry_init()
{
    nui_nentry_tcl_output();
}
