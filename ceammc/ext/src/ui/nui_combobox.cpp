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
#include "nui_combobox.h"
#include "ceammc_format.h"
#include "fmt/core.h"
#include "nui/combobox_view.h"
#include "nui/factory.h"
#include "nui/tk_view_impl.h"

using namespace ceammc::ui;

NUIComboBoxBase::NUIComboBoxBase(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

NUIComboBox::NUIComboBox(const PdArgs& args)
    : Widget<NUIComboBoxBase>(args)
    , box_view_(&box_model_, BoxView::ViewImplPtr(new TclBoxImpl()))
{
    items_ = new ListProperty("@items", {});
    items_->setArgIndex(0);
    addProperty(items_);

    box_model_.data().setInlets(Xlets::fromInlets(owner()));
    box_model_.data().setOutlets(Xlets::fromOutlets(owner()));

    using sc = StyleCollection;
    auto sz = sc::size(0, "combobox:size"_hash, Size(60, 20));

    ViewPtr view(new ComboBoxView(&model_, ComboBoxView::ViewImplPtr(new TclComboBoxImpl()), {}));
    view->setSize(sz);
    box_view_.appendChild(std::move(view));

    setSize(sz);
    setResizeMode(RESIZE_WIDTH);
}

void NUIComboBox::initDone()
{
    for (auto& x : items_->value()) {
        t_symbol* sym = &s_;
        if (x.isSymbol()) {
            sym = x.asT<t_symbol*>();
        } else if (x.isFloat()) {
            sym = gensym(fmt::format("{}", x.asFloat()).c_str());
        } else {
            OBJ_ERR << fmt::format("invalid item value: {}, skipping", to_string(x));
            continue;
        }

        model_.data().valuesRef().push_back(sym);
    }

    Widget<NUIComboBoxBase>::initDone();
}

void NUIComboBox::onBang()
{
    auto idx = model_.data().index();

    if (idx >= 0 && idx < model_.data().values().size()) {
        symbolTo(1, model_.data().values()[idx]);
        floatTo(0, idx);
    }
}

void NUIComboBox::onFloat(t_float f)
{
    const auto N = model_.data().values().size();

    int idx = f;
    if (idx < -1) {
        OBJ_ERR << fmt::format("invalid menu index: {}", idx);
        return;
    } else if (idx >= N) {
        OBJ_ERR << fmt::format("invalid menu index: {}, expected value <{}", idx, N);
        return;
    }

    model_.data().setIndex(idx);
    model_.notify();

    onBang();
}

void NUIComboBox::onWidgetShow()
{
    box_view_.setSize(size());
    box_view_.create(drawCanvasId(), ownerId(), zoom());
}

void NUIComboBox::onWidgetSelect(bool state)
{
    box_model_.data().setBorderColor(state ? colors::blue : colors::st_border);
    box_model_.notify();
}

void NUIComboBox::onWidgetDelete()
{
    box_view_.erase();
}

void NUIComboBox::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    box_view_.acceptEvent(EVENT_MOUSE_DOWN, pt, {});
}

void NUIComboBox::onWidgetResize(const Size& new_sz)
{
    box_view_.setSize(new_sz);
    box_view_.getChild<ComboBoxView>()->setSize(new_sz);
    box_view_.redraw();
}

void NUIComboBox::m_open(t_symbol*, const AtomListView& lv)
{
    bool open = lv.boolAt(0, true);
    model_.data().setIsOpen(open);
    model_.notify();
}

void NUIComboBox::m_next(t_symbol*, const AtomListView& lv)
{
    if (model_.data().index() + 1 < model_.data().values().size()) {
        model_.data().indexRef() += 1;
        model_.notify();
    }
}

void NUIComboBox::m_prev(t_symbol*, const AtomListView& lv)
{
    if (model_.data().index() > 0) {
        model_.data().indexRef() -= 1;
        model_.notify();
    }
}

void NUIComboBox::m_begin(t_symbol*, const AtomListView& lv)
{
    if (model_.data().values().size() > 0) {
        model_.data().indexRef() = 0;
        model_.notify();
    }
}

void NUIComboBox::m_end(t_symbol*, const AtomListView& lv)
{
    if (model_.data().values().size() > 0) {
        model_.data().indexRef() = model_.data().values().size() - 1;
        model_.notify();
    }
}

void setup_nui_combobox()
{
    using namespace ceammc;
    using namespace ceammc::ui;

    UIFactory<ObjectFactory, NUIComboBox> obj("nui.menu");
    obj.addAlias("nmenu");

    obj.useMouseDown();
    obj.useMouseEnter();
    obj.useMouseLeave();
    obj.useMouseMove();
    obj.useMouseRight();
    obj.useMouseUp();

    obj.addMethod("open", &NUIComboBox::m_open);

    obj.addMethod("next", &NUIComboBox::m_next);
    obj.addMethod("prev", &NUIComboBox::m_prev);

    obj.addMethod("begin", &NUIComboBox::m_prev);
    obj.addMethod("end", &NUIComboBox::m_prev);
}
