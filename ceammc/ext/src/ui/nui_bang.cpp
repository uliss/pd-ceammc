#include "nui_bang.h"
#include "ceammc_factory.h"
#include "nui/button_view.h"
#include "nui/factory.h"
#include "nui/tk_view_impl.h"

namespace ceammc {
using namespace ceammc::ui;

NUIBang::NUIBang(const PdArgs& args)
    : ui::Widget<NUIBangBase>(args)
    , clock_([this] { })
    , box_view_(&box_model_, BoxView::ViewImplPtr(new TclBoxImpl()))
{
    // setSize(Size(14, 14));
    // setResizeMode(RESIZE_LINKED);

    // model_.data().
    // model_.data().setOutlets(Xlets::fromOutlets(owner()));

    // using sc = StyleCollection;
    // auto sz = sc::size(0, "button:size"_hash, Size(20, 20));

    box_model_.data().setInlets(Xlets::fromInlets(owner()));
    box_model_.data().setOutlets(Xlets::fromOutlets(owner()));

    using sc = StyleCollection;
    auto sz = sc::size(0, "button:size"_hash, Size(30, 30));

    ViewPtr view(new ButtonView(&model_, ButtonView::ViewImplPtr(new TclButtonImpl()), {}));
    view->setSize(sz);
    box_view_.appendChild(std::move(view));

    setSize(sz);
    setResizeMode(RESIZE_LINKED);
}

// void NUIBang::buildUI()
// {
//     view_.layout();
// }

void NUIBang::onWidgetShow()
{
    box_view_.setSize(size());
    box_view_.create(drawCanvasId(), ownerId(), zoom());
}

void NUIBang::onWidgetSelect(bool state)
{
    box_model_.data().setBorderColor(state ? colors::blue : colors::st_border);
    box_model_.notify();
}

void NUIBang::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    OBJ_ERR << __FUNCTION__;

    model_.data().setState(true);
    model_.notify();

    bangTo(0);
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

    UIFactory<ObjectFactory, NUIBang> obj("nui.bang");
    obj.addAlias("nb");
    obj.useMouseDown();
}
