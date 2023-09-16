#include "nui_bang.h"
#include "ceammc_factory.h"
#include "nui/factory.h"

namespace ceammc {
using namespace ceammc::ui;

NUIBang::NUIBang(const PdArgs &args)
    : ui::Widget<NUIBangBase>(args),
      clock_([this]{}), view_(&model_, ButtonView::ViewImplPtr(new TclButtonImpl()), {})
{
    setSize(Size(14, 14));
    setResizeMode(RESIZE_LINKED);
}

void NUIBang::buildUI()
{
    view_.layout();
}

void NUIBang::onWidgetShow()
{
    view_.create((WinId)drawCanvas(), (WidgetId)owner(), zoom());
    view_.setSize(size());
}

void NUIBang::onMouseDown(const Point &pt, const Point &abspt, uint32_t mod)
{
    OBJ_ERR << __FUNCTION__;
//    view_.onEvent()
}


}

void setup_nui_bang()
{
    using namespace ceammc;
    using namespace ceammc::ui;

    UIFactory<ObjectFactory, NUIBang> obj("nui.bang");
    obj.addAlias("nb");
    obj.useMouseDown();
}

