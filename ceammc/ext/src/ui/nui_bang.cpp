#include "nui_bang.h"
#include "nui/button_view.h"
#include "nui/widget_factory.h"

namespace ceammc {
using namespace ceammc::ui;

NUIBang::NUIBang(const PdArgs& args)
    : ui::SimpleTclWidget<NUIBangBase>(args)
    , clock_([this] {
        model_.data().setState(false);
        model_.notify();
    })
{
    using sc = StyleCollection;
    auto sz = sc::size(0, "button:size"_hash, Size(30, 30));

    ViewPtr view(new ButtonView(&model_, ButtonView::ViewImplPtr(new TclButtonImpl()), {}));
    view->setSize(sz);
    setModelView(std::move(view));

    setSize(sz);
    setResizeMode(RESIZE_LINKED);
}

void NUIBang::onBang()
{
    model_.data().setState(true);
    model_.notify();
    clock_.delay(100);
    bangTo(0);
}

void NUIBang::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    onBang();
}

NUIBangBase::NUIBangBase(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

} // namespace ceammc

void setup_nui_bang()
{
    using namespace ceammc;
    using namespace ceammc::ui;

    WidgetFactory<NUIBang> obj("nui.bang");
    obj.addAlias("nb");
}
