#ifndef CEAMMC_CICM_H
#define CEAMMC_CICM_H

#include "cicm/Sources/cicm_wrapper.h"

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace ceammc {

class UILayer;

class ColorRGBA : public t_rgba {
public:
    ColorRGBA(float r = 0, float g = 0, float b = 0, float a = 1);
    ColorRGBA(const ColorRGBA& c);
    ColorRGBA(const t_rgba& c);
    void operator=(const ColorRGBA& c);

    t_rgba& rgba();
    const t_rgba& rgba() const;

public:
    static ColorRGBA black();
    static ColorRGBA white();
    static ColorRGBA red();
    static ColorRGBA green();
    static ColorRGBA blue();
};

enum FontDecoration {
    FONT_DECORATION_NONE = 0,
    FONT_DECORATION_ITALIC = 1
};

enum FontWeight {
    FONT_WEIGHT_NORMAL = 0,
    FONT_WEIGHT_BOLD = 1
};

class UIFont {
    t_efont* font_;

public:
    UIFont(t_symbol* family, int size,
        FontDecoration decoration = FONT_DECORATION_NONE,
        FontWeight w = FONT_WEIGHT_NORMAL);

    ~UIFont() noexcept;

    UIFont(const UIFont& font);
    void operator=(const UIFont& font);

    t_efont* font();

    int size() const { return font_->c_sizereal; }
    void setSize(int sz)
    {
        font_->c_sizereal = sz;
        font_->c_size = sz;
    }
};

class UITextLayout {
    t_etext* text_;
    t_efont* font_;
    etextanchor_flags anchor_;
    etextjustify_flags justify_;
    etextwrap_flags wrap_;
    float x_, y_, w_, h_;

    UITextLayout(const UITextLayout&);
    void operator=(const UITextLayout&);

public:
    UITextLayout(t_efont* font,
        const ColorRGBA& c = ColorRGBA::black(),
        etextanchor_flags anchor = ETEXT_LEFT,
        etextjustify_flags justify = ETEXT_JLEFT,
        etextwrap_flags wrap = ETEXT_NOWRAP) noexcept;
    UITextLayout(UITextLayout&& l) noexcept;

    ~UITextLayout() noexcept;

    ColorRGBA color() const;
    void setColor(const ColorRGBA& c);
    t_efont* font() { return font_; }
    void setFont(t_efont* f) { font_ = f; }
    void set(const char* txt, float x, float y, float width, float height);
    void set(const char* txt);

    void setX(float x);
    void setY(float y);
    void setPos(float x, float y);
    void setWidth(float w);
    void setHeight(float h);
    void setSize(float w, float h);
    void setJustify(etextjustify_flags j);
    void setAnchor(etextanchor_flags a);
    void setWrap(bool v);

    const char* text() const { return text_->c_text; }

public:
    friend class UILayer;
    friend class UIPainter;
};

class UIPainter {
    t_ebox* parent_;
    t_symbol* name_;
    t_elayer* layer_;

public:
    UIPainter(t_ebox* box, t_symbol* name, const t_rect& brect);
    ~UIPainter();

    operator bool() const { return layer_ != 0; }

    void drawText(const UITextLayout& txt);
    void drawCircleArc(float cx, float cy, float r, float angle0, float angle1);
    void drawLine(float x0, float y0, float x1, float y1);
    void drawRect(float x, float y, float w, float h);
    void drawRect(const t_rect& r);
    void drawCircle(float x, float y, float r);
    void drawPoly(const std::vector<t_pt>& v);

    void drawLineTo(float x, float y);
    void moveTo(float x, float y);

    void fill();
    void fillPreserve();
    void stroke();
    void strokePreserve();
    void closePath();

    void fillLayer(const t_rgba& color);

    void setCapStyle(t_capstyle style);
    void setLineWidth(float w);
    void setColor(const t_rgba& c);
    void setDashStyle(t_dashstyle style);
    void setSmooth(t_smooth smooth);

    t_elayer* layer();
    void raiseOver(UIPainter& painter);

    void rotate(float angle);
    void setMatrix(const t_matrix& mtx);
    void preAllocObjects(size_t n);
    void preAllocPoints(size_t n);

    void optimizeLines(bool v);
};

class UILayer {
    t_ebox* parent_;
    t_symbol* name_;

public:
    UILayer(t_ebox* box, t_symbol* name);
    UIPainter painter(const t_rect& r);
    void invalidate();
};

bool contains_point(const t_rect& r, const t_pt& pt);

class PopupMenuCallbacks {
public:
    using MenuEntryFn = std::function<void(const t_pt&)>;
    using Entry = std::tuple<std::string, MenuEntryFn>;
    using MenuItems = std::vector<Entry>;

private:
    MenuItems items_;
    std::string name_;

public:
    PopupMenuCallbacks(const std::string& name, std::initializer_list<Entry> args = {});
    MenuItems& items() { return items_; }
    const MenuItems& items() const { return items_; }
    const std::string& name() const { return name_; }

    void addSeparator();
    void addItem(const std::string& name, MenuEntryFn fn);

    bool process(t_symbol* name, size_t idx, const t_pt& pt);

public:
    static Entry sep()
    {
        return std::make_tuple(std::string(), MenuEntryFn([](const t_pt&) {}));
    }
};

class UIPopupMenu {
    t_epopup* menu_;
    t_pt abs_pos_;
    t_pt rel_pos_;
    PopupMenuCallbacks menu_items_;
    std::vector<std::string> disabled_items_;

public:
    UIPopupMenu(t_eobj* x,
        const PopupMenuCallbacks& items,
        const t_pt& absPos,
        const t_pt& relPos);

    ~UIPopupMenu();

    void disable(const std::string& name);
    void disable(const std::vector<std::string>& names);
};

}

#endif // CEAMMC_CICM_H
