#include "nui_button.h"
#include "nui/button_view.h"
#include "nui/widget_factory.h"

namespace ceammc {
using namespace ceammc::ui;

NUIButtonBase::NUIButtonBase(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

NUIButton::NUIButton(const PdArgs& args)
    : ui::SimpleTclWidget<NUIButtonBase>(args)
{
    using sc = StyleCollection;
    auto sz = sc::size(0, "button:size"_hash, Size(30, 30));

    ViewPtr view(new ButtonView(&model_, ButtonView::ViewImplPtr(new TclButtonImpl()), {}));
    view->setSize(sz);
    setModelView(std::move(view));

    setSize(sz);
    setResizeMode(RESIZE_LINKED);
}

void NUIButton::onBang()
{
    floatTo(0, model_.data().state() //
            ? model_.data().onValue()
            : model_.data().offValue());
}

void NUIButton::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    EventContext ctx;
    ctx.modifiers = mod;

    if (boxView().acceptEvent(EVENT_MOUSE_DOWN, pt, ctx).isAccepted())
        onBang();
}

void NUIButton::onMouseUp(const Point& pt, uint32_t mod)
{
    EventContext ctx;
    ctx.modifiers = mod;

    if (boxView().acceptEvent(EVENT_MOUSE_UP, pt, ctx).isAccepted())
        onBang();
}

} // namespace ceammc

void setup_nui_button()
{
    using namespace ceammc;
    using namespace ceammc::ui;

    WidgetFactory<NUIButton> obj("nui.button");
    obj.addAlias("nbtn");
}
