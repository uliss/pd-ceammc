#include "nui_toggle.h"
#include "ceammc_factory.h"
#include "nui/factory.h"
#include "nui/tk_view_impl.h"

namespace ceammc {
using namespace ceammc::ui;

NUIToggle::NUIToggle(const PdArgs& args)
    : ui::Widget<NUIToggleBase>(args)
    , box_view_(&box_model_, BoxView::ViewImplPtr(new TclBoxImpl()))
{
    box_model_.data().setInlets(Xlets::fromInlets(owner()));
    box_model_.data().setOutlets(Xlets::fromOutlets(owner()));

    using sc = StyleCollection;
    auto sz = sc::size(0, "toggle:size"_hash, Size(30, 30));

    ViewPtr view(new ToggleView(&model_, ToggleView::ViewImplPtr(new TclToggleImpl()), {}));
    view->setSize(sz);
    box_view_.appendChild(std::move(view));

    setSize(sz);
    setResizeMode(RESIZE_LINKED);
}

void NUIToggle::onFloat(t_float f)
{
    model_.data().setValue(f);
    model_.notify();
    floatTo(0, f);
}

void NUIToggle::onWidgetShow()
{
    box_view_.setSize(size());
    box_view_.create((WinId)drawCanvas(), (WidgetId)owner(), zoom());
}

void NUIToggle::onWidgetSelect(bool state)
{
    box_model_.data().setBorderColor(state ? colors::blue : colors::st_border);
    box_model_.notify();
}

void NUIToggle::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    box_view_.getChild<ToggleView>()->onEvent(EVENT_MOUSE_DOWN, pt, {});
    floatTo(0, model_.data().value());
}

void NUIToggle::onWidgetResize(const Size &new_sz)
{
    OBJ_ERR << __FUNCTION__;
    box_view_.setSize(new_sz);
    box_view_.getChild<ToggleView>()->setSize(new_sz);
    box_view_.redraw();
    box_view_.getChild<ToggleView>()->redraw();
//    box_model_.notify();
//    model_.notify();
}

NUIToggleBase::NUIToggleBase(const PdArgs &args)
    : BaseObject(args)
{
    createOutlet();
}

}

void setup_nui_toggle()
{
    using namespace ceammc;
    using namespace ceammc::ui;

    UIFactory<ObjectFactory, NUIToggle> obj("nui.toggle");
    obj.addAlias("nt");

    obj.useMouseDown();
    obj.useMouseMove();
}
