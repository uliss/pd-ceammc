#ifndef CEAMMC_CICM_H
#define CEAMMC_CICM_H

#include "cicm/Sources/cicm_wrapper.h"

#include <memory>
#include <string>
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
    t_symbol* family_;
    int size_;
    FontDecoration dec_;
    FontWeight w_;

public:
    UIFont(t_symbol* family, int size,
        FontDecoration decoration = FONT_DECORATION_NONE,
        FontWeight w = FONT_WEIGHT_NORMAL);

    ~UIFont();

    UIFont(const UIFont& font);
    void operator=(const UIFont& font);

    t_efont* font();
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
        etextwrap_flags wrap = ETEXT_NOWRAP);

    ~UITextLayout();

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

class UIPopupMenu {
    t_epopup* menu_;
    t_pt abs_pos_;
    t_pt rel_pos_;
    typedef std::pair<std::string, bool> MenuEntry;
    std::vector<MenuEntry> menu_items_;

public:
    UIPopupMenu(t_eobj* x, const char* name, const t_pt& absPos, const t_pt& relPos);
    UIPopupMenu(t_eobj* x, t_symbol* name, const t_pt& absPos, const t_pt& relPos);
    ~UIPopupMenu();

    void addSeparator();
    void addItem(const std::string& name, bool enabled = true);
};

}

#endif // CEAMMC_CICM_H
