#include "nui_help_box.h"
#include "nui/help_box_view.h"
#include "nui/widget_factory.h"

namespace ceammc {
using namespace ceammc::ui;

NUIHelpBoxBase::NUIHelpBoxBase(const PdArgs& args)
    : BaseObject(args)
{
}

NUIHelpBox::NUIHelpBox(const PdArgs& args)
    : ui::SimpleTclWidget<NUIHelpBoxBase>(args)
{
    using sc = StyleCollection;
    auto sz = sc::size(0, "help_box:size"_hash, Size(20, 16));

    ViewPtr view(new HelpboxView(&model_, HelpboxView::ViewImplPtr(new TclHelpboxImpl()), {}));
    view->setSize(sz);
    setModelView(std::move(view));

    setSize(sz);
    setResizeMode(RESIZE_WIDTH);
}

void NUIHelpBox::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    EventContext ctx;
    ctx.modifiers = mod;

    if (boxView().acceptEvent(EVENT_MOUSE_DOWN, pt, ctx).isAccepted()) {
        OBJ_DBG << "click";
        // model_.data().setIsOpen()
    }
}

void NUIHelpBox::onMouseUp(const Point& pt, uint32_t mod)
{
    EventContext ctx;
    ctx.modifiers = mod;

    if (boxView().acceptEvent(EVENT_MOUSE_UP, pt, ctx).isAccepted())
        onBang();
}

} // namespace ceammc

void setup_nui_help_box()
{
    using namespace ceammc::ui;

    WidgetFactory<NUIHelpBox> obj("nui.helpbox", ceammc::OBJECT_FACTORY_NO_DEFAULT_INLET);
}
