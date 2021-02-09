/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef LANG_FAUST_UI_TILDE_H
#define LANG_FAUST_UI_TILDE_H

#include "ceammc_sound_external.h"
using namespace ceammc;

#include <algorithm>

template <typename T>
class Size {
    T w_, h_;

public:
    Size(T w, T h)
        : w_(w)
        , h_(h)
    {
    }

    Size<T> operator*(T x) const { return Size<T>(w_ * x, h_ * x); }

    T width() const { return w_; }
    T height() const { return h_; }
};

template <typename T>
class Rect {
    T x0_, y0_, x1_, y1_;

public:
    Rect(T x0, T y0, T x1, T y1)
        : x0_(x0)
        , y0_(y0)
        , x1_(x1)
        , y1_(y1)
    {
    }

    Rect(T x, T y, const Size<T>& sz)
        : x0_(x)
        , y0_(y)
        , x1_(x + sz.width())
        , y1_(y + sz.height())
    {
    }

    void normalize()
    {
        std::tie(x0_, x1_) = std::minmax<T>(x0_, x1_);
        std::tie(y0_, y1_) = std::minmax<T>(y0_, y1_);
    }

    T width() const { return x1_ - x0_; }
    T height() const { return y1_ - y0_; }
    T left() const { return x0_; }
    T top() const { return y0_; }
    T right() const { return x1_; }
    T bottom() const { return y1_; }

    Size<T> size() const { return Size<T>(width(), height()); }
};

class WidgetIFace {
    t_object* x_;
    t_canvas* widget_canvas_;
    Size<int> size_;

public:
    WidgetIFace(t_object* x, t_glist* widget_canvas);
    // pure virtual
    virtual ~WidgetIFace() = 0;
    virtual Rect<int> getRealRect(t_glist* cnv) const;
    virtual Rect<int> getRect(t_glist* cnv) const;
    virtual void displace(t_glist* cnv, int dx, int dy);

    // draw functions
    void drawMove(t_glist* cnv);
};

class LangFaustUiTilde : public SoundExternal, public WidgetIFace {

public:
    LangFaustUiTilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) final;
};

void setup_lang_faust_ui_tilde();

#endif // LANG_FAUST_UI_TILDE_H
