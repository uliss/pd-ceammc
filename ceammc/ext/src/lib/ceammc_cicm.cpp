#include "ceammc_cicm.h"

#include <algorithm>

namespace ceammc {

static const char* SYM_DECORATION_NONE = "roman";
static const char* SYM_DECORATION_ITALIC = "italic";
static const char* SYM_WEIGHT_NORMAL = "normal";
static const char* SYM_WEIGHT_BOLD = "bold";

static t_symbol* toSymbol(FontDecoration dec)
{
    switch (dec) {
    case FONT_DECORATION_NONE:
        return gensym(SYM_DECORATION_NONE);
    case FONT_DECORATION_ITALIC:
    default:
        return gensym(SYM_DECORATION_ITALIC);
    }
}

static t_symbol* toSymbol(FontWeight w)
{
    switch (w) {
    case FONT_WEIGHT_NORMAL:
        return gensym(SYM_WEIGHT_NORMAL);
    case FONT_WEIGHT_BOLD:
    default:
        return gensym(SYM_WEIGHT_BOLD);
    }
}

UIFont::UIFont(t_symbol* family, int size, FontDecoration decoration, FontWeight w)
    : font_(efont_create(family, toSymbol(decoration), toSymbol(w), size))
{
}

UIFont::UIFont(const UIFont& font)
    : font_(efont_create(font.font_->c_family, font.font_->c_slant, font.font_->c_weight, font.font_->c_sizereal))
{
}

void UIFont::operator=(const UIFont& font)
{
    // self assignment
    if (this == &font)
        return;

    efont_destroy(font_);
    font_ = efont_create(font.font_->c_family, font.font_->c_slant, font.font_->c_weight, font.font_->c_sizereal);
}

UIFont::~UIFont() noexcept
{
    efont_destroy(font_);
}

t_efont* UIFont::font() { return font_; }

UITextLayout::UITextLayout(t_efont* font,
    const ColorRGBA& c,
    etextanchor_flags anchor,
    etextjustify_flags justify,
    etextwrap_flags wrap) noexcept

    : text_(nullptr)
    , font_(font)
    , anchor_(anchor)
    , justify_(justify)
    , wrap_(wrap)
    , x_(0)
    , y_(0)
    , w_(100)
    , h_(font_->c_size)
{
    text_ = etext_layout_create();
    set("");
    ColorRGBA color(c);
    etext_layout_settextcolor(text_, &color);
}

UITextLayout::UITextLayout(UITextLayout&& l) noexcept
    : text_(l.text_)
    , font_(l.font_)
    , anchor_(l.anchor_)
    , justify_(l.justify_)
    , wrap_(l.wrap_)
    , x_(l.x_)
    , y_(l.y_)
    , w_(l.w_)
    , h_(l.h_)
{
    l.text_ = nullptr;
    l.font_ = nullptr;
    l.x_ = -1;
    l.y_ = -1;
    l.w_ = 0;
    l.h_ = 0;
}

UITextLayout::~UITextLayout() noexcept
{
    if (text_)
        etext_layout_destroy(text_);
}

ColorRGBA UITextLayout::color() const
{
    return text_->c_color;
}

void UITextLayout::setColor(const ColorRGBA& c)
{
    text_->c_color = c.rgba();
}

void UITextLayout::set(const char* txt, float x, float y, float width, float height)
{
    x_ = x;
    y_ = y;
    w_ = width;
    h_ = height;

    set(txt);
}

void UITextLayout::set(const char* txt)
{
    etext_layout_set(text_, txt, font_, x_, y_, w_, h_, anchor_, justify_, wrap_);
}

void UITextLayout::setX(float x)
{
    x_ = x;
    text_->c_rect.x = x;
}

void UITextLayout::setY(float y)
{
    y_ = y;
    text_->c_rect.y = y;
}

void UITextLayout::setPos(float x, float y)
{
    x_ = x;
    y_ = y;
    text_->c_rect.x = x;
    text_->c_rect.y = y;
}

void UITextLayout::setWidth(float w)
{
    w_ = w;
    text_->c_rect.width = w;
}

void UITextLayout::setHeight(float h)
{
    h_ = h;
    text_->c_rect.height = h;
}

void UITextLayout::setSize(float w, float h)
{
    w_ = w;
    h_ = h;
    text_->c_rect.width = w;
    text_->c_rect.height = h;
}

void UITextLayout::setJustify(etextjustify_flags j)
{
    justify_ = j;
}

void UITextLayout::setAnchor(etextanchor_flags a)
{
    anchor_ = a;
}

void UITextLayout::setWrap(bool v)
{
    wrap_ = v ? ETEXT_WRAP : ETEXT_NOWRAP;
}

ColorRGBA::ColorRGBA(float r, float g, float b, float a)
{
    t_rgba::red = r;
    t_rgba::green = g;
    t_rgba::blue = b;
    t_rgba::alpha = a;
}

ColorRGBA::ColorRGBA(const ColorRGBA& c)
{
    t_rgba::red = c.t_rgba::red;
    t_rgba::green = c.t_rgba::green;
    t_rgba::blue = c.t_rgba::blue;
    t_rgba::alpha = c.t_rgba::alpha;
}

ColorRGBA::ColorRGBA(const t_rgba& c)
{
    t_rgba::red = c.red;
    t_rgba::green = c.green;
    t_rgba::blue = c.blue;
    t_rgba::alpha = c.alpha;
}

void ColorRGBA::operator=(const ColorRGBA& c)
{
    t_rgba::red = c.t_rgba::red;
    t_rgba::green = c.t_rgba::green;
    t_rgba::blue = c.t_rgba::blue;
    t_rgba::alpha = c.t_rgba::alpha;
}

t_rgba& ColorRGBA::rgba()
{
    return *this;
}

const t_rgba& ColorRGBA::rgba() const
{
    return *this;
}

ColorRGBA ColorRGBA::black() { return ColorRGBA(0, 0, 0); }
ColorRGBA ColorRGBA::white() { return ColorRGBA(1, 1, 1); }
ColorRGBA ColorRGBA::red() { return ColorRGBA(1, 0, 0); }
ColorRGBA ColorRGBA::green() { return ColorRGBA(0, 1, 0); }
ColorRGBA ColorRGBA::blue() { return ColorRGBA(0, 0, 1); }

UILayer::UILayer(t_ebox* box, t_symbol* name)
    : parent_(box)
    , name_(name)
{
}

void UILayer::invalidate()
{
    ebox_invalidate_layer(parent_, name_);
}

UIPainter UILayer::painter(const t_rect& r)
{
    return UIPainter(parent_, name_, r);
}

bool contains_point(const t_rect& r, const t_pt& pt)
{
    const float dx = pt.x - r.x;
    const float dy = pt.y - r.y;
    return dx >= 0 && dy >= 0 && dx <= r.width && dy <= r.height;
}

UIPainter::UIPainter(t_ebox* box, t_symbol* name, const t_rect& brect)
    : parent_(box)
    , name_(name)
    , layer_(0)
{
    layer_ = ebox_start_layer(parent_, name_, brect.width, brect.height);
}

UIPainter::~UIPainter()
{
    if (layer_) {
        ebox_end_layer(parent_, name_);
        layer_->e_optimize = false;
    }

    ebox_paint_layer(parent_, name_, 0, 0);
}

void UIPainter::drawText(const UITextLayout& txt)
{
    etext_layout_draw(txt.text_, layer_);
}

void UIPainter::drawCircleArc(float cx, float cy, float r, float angle0, float angle1)
{
    egraphics_arc_oval(layer_, cx, cy, r, r, angle0, angle1);
    egraphics_stroke(layer_);
}

void UIPainter::drawLine(float x0, float y0, float x1, float y1)
{
    egraphics_line_fast(layer_, x0, y0, x1, y1);
}

void UIPainter::drawRect(float x, float y, float w, float h)
{
    egraphics_rectangle(layer_, x, y, w, h);
}

void UIPainter::drawRect(const t_rect& r)
{
    egraphics_rectangle(layer_, r.x, r.y, r.width, r.height);
}

void UIPainter::drawCircle(float x, float y, float r)
{
    egraphics_circle(layer_, x, y, r);
}

void UIPainter::drawPoly(const std::vector<t_pt>& v)
{
    egraphics_poly(layer_, v);
}

void UIPainter::drawLineTo(float x, float y)
{
    egraphics_line_to(layer_, x, y);
}

void UIPainter::moveTo(float x, float y)
{
    egraphics_move_to(layer_, x, y);
}

void UIPainter::fill()
{
    egraphics_fill(layer_);
}

void UIPainter::fillPreserve()
{
    egraphics_fill_preserve(layer_);
}

void UIPainter::stroke()
{
    egraphics_stroke(layer_);
}

void UIPainter::strokePreserve()
{
    egraphics_stroke_preserve(layer_);
}

void UIPainter::closePath()
{
    egraphics_close_path(layer_);
}

void UIPainter::fillLayer(const t_rgba& color)
{
    float w = layer_->e_rect.width;
    float h = layer_->e_rect.height;

    setColor(color);
    drawRect(0, 0, w, h);
    fill();
}

void UIPainter::setCapStyle(t_capstyle style)
{
    egraphics_set_line_capstyle(layer_, style);
}

void UIPainter::setLineWidth(float w)
{
    egraphics_set_line_width(layer_, w);
}

void UIPainter::setColor(const t_rgba& c)
{
    egraphics_set_color_rgba(layer_, &c);
}

void UIPainter::setDashStyle(t_dashstyle style)
{
    egraphics_set_line_dashstyle(layer_, style);
}

void UIPainter::setSmooth(t_smooth smooth)
{
    egraphics_set_line_smooth(layer_, smooth);
}

t_elayer* UIPainter::layer()
{
    return layer_;
}

void UIPainter::raiseOver(UIPainter& painter)
{
    if (!painter)
        return;

    egraphics_raise(layer_, painter.layer());
}

void UIPainter::rotate(float angle)
{
    egraphics_rotate(layer_, angle);
}

void UIPainter::setMatrix(const t_matrix& mtx)
{
    egraphics_set_matrix(layer_, &mtx);
}

void UIPainter::preAllocObjects(size_t n)
{
    egraphics_preallocate_objects(layer_, n);
}

void UIPainter::preAllocPoints(size_t n)
{
    egraphics_preallocate_points(layer_, n);
}

void UIPainter::optimizeLines(bool v)
{
    layer_->e_optimize = v;
}

UIPopupMenu::UIPopupMenu(t_eobj* x,
    const PopupMenuCallbacks& items,
    const t_pt& absPos,
    const t_pt& relPos)
    : menu_(nullptr)
    , abs_pos_(absPos)
    , rel_pos_(relPos)
    , menu_items_(items)
{
    menu_ = epopupmenu_create(x, gensym(items.name().c_str()));
}

UIPopupMenu::~UIPopupMenu()
{
    int cnt = 0;
    for (auto& m : menu_items_.items()) {
        if (std::get<0>(m).empty())
            epopupmenu_addseparator(menu_);
        else {
            auto it = std::find(disabled_items_.begin(), disabled_items_.end(), std::get<0>(m));
            epopupmenu_additem(menu_, cnt, std::get<0>(m).c_str(), it == disabled_items_.end(), rel_pos_);
        }

        // counter increment
        cnt++;
    }

    epopupmenu_popup(menu_, abs_pos_);

    free(menu_);
}

void UIPopupMenu::disable(const std::string& name)
{
    disabled_items_.push_back(name);
}

void UIPopupMenu::disable(const std::vector<std::string>& names)
{
    disabled_items_.insert(disabled_items_.end(), names.begin(), names.end());
}

PopupMenuCallbacks::PopupMenuCallbacks(const std::string& name,
    std::initializer_list<PopupMenuCallbacks::Entry> args)
    : name_(name)
    , items_(args)
{
}

void PopupMenuCallbacks::addSeparator()
{
    items_.emplace_back("", MenuEntryFn());
}

void PopupMenuCallbacks::addItem(const std::string& name, PopupMenuCallbacks::MenuEntryFn fn)
{
    items_.emplace_back(name, fn);
}

bool PopupMenuCallbacks::process(t_symbol* name, size_t idx, const t_pt& pt)
{
    if (name_ != name->s_name)
        return false;

    if (idx >= items_.size())
        return false;

    if (!std::get<1>(items_[idx]))
        return false;

    auto fn = std::get<1>(items_[idx]);
    fn(pt);
    return true;
}

}
