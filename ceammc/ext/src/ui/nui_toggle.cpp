#include "nui_toggle.h"
#include "nui/tk_view_impl.h"
#include "nui/widget_factory.h"

namespace ceammc {
using namespace ceammc::ui;

NUIToggleBase::NUIToggleBase(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

NUIToggle::NUIToggle(const PdArgs& args)
    : ui::SimpleTclWidget<NUIToggleBase>(args)
{
    using sc = StyleCollection;
    auto sz = sc::size(0, "toggle:size"_hash, Size(30, 30));

    ViewPtr view(new ToggleView(&model_, ToggleView::ViewImplPtr(new TclToggleImpl()), {}));
    view->setSize(sz);
    setModelView(std::move(view));

    setSize(sz);
    setResizeMode(RESIZE_LINKED);
}

void NUIToggle::onBang()
{
    model_.data().setState(!model_.data().state());
    model_.notify();
    output();
}

void NUIToggle::onFloat(t_float f)
{
    model_.data().setState(f != 0);
    model_.notify();
    output();
}

void NUIToggle::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    boxView().acceptEvent(EVENT_MOUSE_DOWN, pt, {});
    output();
}

void NUIToggle::output()
{
    floatTo(0, model_.data().state() //
            ? model_.data().onValue()
            : model_.data().offValue());
}

} // namespace ceammc

void setup_nui_toggle()
{
    using namespace ceammc;
    using namespace ceammc::ui;

    WidgetFactory<NUIToggle> obj("nui.toggle");
    obj.addAlias("nt");
}
