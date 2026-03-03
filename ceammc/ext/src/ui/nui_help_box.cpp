#include "nui_help_box.h"
#include "ceammc_format.h"
#include "nui/help_box_view.h"
#include "nui/widget_factory.h"

namespace {
ceammc::AtomList to_pd_type(const std::vector<std::string>& data)
{
    ceammc::AtomList res;
    res.reserve(data.size());

    for (auto& s : data)
        res.append(gensym(s.c_str()));

    return res;
}
} // namespace

namespace ceammc {
using namespace ceammc::ui;

NUIHelpBoxBase::NUIHelpBoxBase(const PdArgs& args)
    : BaseObject(args)
{
}

NUIHelpBox::NUIHelpBox(const PdArgs& args)
    : ui::SimpleTclWidget<NUIHelpBoxBase>(args)
    , lines_(nullptr)
    , title_(nullptr)
{
    using sc = StyleCollection;
    auto sz = sc::size(0, "help_box:size"_hash, Size(26, 16));

    ViewPtr view(new HelpboxView(&model_, HelpboxView::ViewImplPtr(new TclHelpboxImpl()), {}));
    view->setSize(sz);
    setModelView(std::move(view));

    boxView().model()->data().setBorderColor(model_.data().borderColor());
    boxView().model()->data().setFillColor(model_.data().fillColor());

    setSize(sz);
    setResizeMode(RESIZE_WIDTH);

    lines_ = new ListProperty("@lines");
    lines_->setSuccessFn([this](Property*) {
        auto& lines = model_.data().textLinesRef();
        lines.clear();
        lines.reserve(lines_->value().size());

        for (auto& a : lines_->value())
            lines.push_back(to_string(a));
    });
    addProperty(lines_);

    title_ = new SymbolProperty("@title", &s_);
    title_->setSuccessFn([this](Property*) {
        model_.data().titleRef().assign(title_->cstr());
    });
    addProperty(title_);
}

void NUIHelpBox::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    EventContext ctx;
    ctx.modifiers = mod;

    modelView()->acceptEvent(EVENT_MOUSE_DOWN, pt, ctx);
}

} // namespace ceammc

void setup_nui_help_box()
{
    using namespace ceammc::ui;

    WidgetFactory<NUIHelpBox> obj("nui.helpbox", ceammc::OBJECT_FACTORY_NO_DEFAULT_INLET);
}
