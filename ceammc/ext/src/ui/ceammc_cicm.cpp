#include "ceammc_cicm.h"

namespace ceammc {

static t_symbol* SYM_DECORATION_NONE = gensym("roman");
static t_symbol* SYM_DECORATION_ITALIC = gensym("italic");
static t_symbol* SYM_WEIGHT_NORMAL = gensym("normal");
static t_symbol* SYM_WEIGHT_BOLD = gensym("bold");

static t_symbol* toSymbol(FontDecoration dec)
{
    switch (dec) {
    case FONT_DECORATION_NONE:
        return SYM_DECORATION_NONE;
    case FONT_DECORATION_ITALIC:
    default:
        return SYM_DECORATION_ITALIC;
    }
}

static t_symbol* toSymbol(FontWeight w)
{
    switch (w) {
    case FONT_WEIGHT_NORMAL:
        return SYM_WEIGHT_NORMAL;
    case FONT_WEIGHT_BOLD:
    default:
        return SYM_WEIGHT_BOLD;
    }
}

UIFont::UIFont(t_symbol* family, int size, FontDecoration decoration, FontWeight w)
    : font_(0)
    , family_(family)
    , size_(size)
    , dec_(decoration)
    , w_(w)
{
    font_ = efont_create(family_, toSymbol(dec_), toSymbol(w_), size_);
}

UIFont::UIFont(const UIFont& font)
    : font_(0)
    , family_(font.family_)
    , size_(font.size_)
    , dec_(font.dec_)
    , w_(font.w_)
{
    font_ = efont_create(family_, toSymbol(dec_), toSymbol(w_), size_);
}

void UIFont::operator=(const UIFont& font)
{
    // self assignment
    if (this == &font)
        return;

    efont_destroy(font_);
    family_ = font.family_;
    size_ = font.size_;
    dec_ = font.dec_;
    w_ = font.w_;
    font_ = efont_create(family_, toSymbol(dec_), toSymbol(w_), size_);
}

UIFont::~UIFont()
{
    efont_destroy(font_);
}

t_efont* UIFont::font() { return font_; }

UITextLayout::UITextLayout(t_efont* font,
    const ColorRGBA& c,
    etextanchor_flags anchor,
    etextjustify_flags justify,
    etextwrap_flags wrap)

    : text_(0)
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

UITextLayout::~UITextLayout()
{
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
}

void UITextLayout::setY(float y)
{
    y_ = y;
}

void UITextLayout::setPos(float x, float y)
{
    x_ = x;
    y_ = y;
}

void UITextLayout::setWidth(float w)
{
    w_ = w;
}

void UITextLayout::setHeight(float h)
{
    h_ = h;
}

void UITextLayout::setSize(float w, float h)
{
    w_ = w;
    h_ = h;
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
    return dx > 0 && dy > 0 && dx < r.width && dy < r.height;
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
    if (layer_)
        ebox_end_layer(parent_, name_);

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

t_elayer* UIPainter::layer()
{
    return layer_;
}
}
