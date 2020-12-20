/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "array_window.h"
#include "ceammc_factory.h"
#include "ceammc_window.h"

#include <unordered_map>

static t_symbol* WIN_TRIANGLE;
static t_symbol* WIN_WELCH;
static t_symbol* WIN_HANN;
static t_symbol* WIN_RECT;
static t_symbol* WIN_SINE;
static t_symbol* WIN_HAMMING;
static t_symbol* WIN_BLACKMAN;
static t_symbol* WIN_NUTTALL;
static t_symbol* WIN_BLACKMAN_HARRIS;
static t_symbol* WIN_FLATTOP;
static t_symbol* WIN_GAUSS;
static t_symbol* WIN_DEFAULT;
static t_symbol* PROP_TYPE;

typedef std::unordered_map<t_symbol*, WindowFuncPtr> WFuncMap;
static WFuncMap fn_map;

ArrayWindow::ArrayWindow(const PdArgs& args)
    : ArrayMod(args)
    , type_(nullptr)
    , gen_fn_(window::hann<t_float>)
{
    createOutlet();

    type_ = new SymbolEnumProperty("@type", WIN_DEFAULT);
    type_->setSuccessFn([this](Property* p) { updateGenFn(); });
    for (auto& p : fn_map) {
        if (p.first != WIN_DEFAULT)
            type_->appendEnum(p.first);
    }
    addProperty(type_);
}

void ArrayWindow::onBang()
{
    if (checkArray())
        fill();
}

void ArrayWindow::updateGenFn()
{
    gen_fn_ = fn_map[type_->value()];
    fill();
}

void ArrayWindow::m_resize(t_symbol* s, const AtomListView& l)
{
    if (!checkArgs(l, ARG_NATURAL, s))
        return;

    size_t N = l[0].asSizeT();
    if (N == 0) {
        OBJ_ERR << "invalid size: " << l;
        return;
    }

    if (!array_.update()) {
        OBJ_ERR << "can't access array: " << array_.name();
        return;
    }

    if (!array_.resize(N))
        OBJ_ERR << "can't resize array: " << array_.name();
}

void ArrayWindow::m_fit(t_symbol* s, const AtomListView& /*l*/)
{
    if (!array_.isValid()) {
        OBJ_ERR << "invalid array: " << array_.name();
        return;
    }

    array_.setYBounds(0, 1);
}

void ArrayWindow::m_triangle(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_gauss(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_hann(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_rect(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_sine(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_hamming(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_blackman(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_blackman_harris(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_flattop(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_welch(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::m_nuttall(t_symbol* s, const AtomListView& args)
{
    type_->setValue(s);
    updateGenFn();
}

void ArrayWindow::fill()
{
    if (!array_.update())
        return;

    const auto N = array_.size();

    for (size_t i = 0; i < N; i++)
        array_[i] = gen_fn_(i, N);

    if (shouldRedraw())
        array_.redraw();

    bangTo(0);
}

void setup_array_window()
{
    WIN_TRIANGLE = gensym("tri");
    WIN_WELCH = gensym("welch");
    WIN_HANN = gensym("hann");
    WIN_RECT = gensym("rect");
    WIN_SINE = gensym("sine");
    WIN_HAMMING = gensym("hamming");
    WIN_BLACKMAN = gensym("blackman");
    WIN_NUTTALL = gensym("nuttall");
    WIN_BLACKMAN_HARRIS = gensym("blackman-harris");
    WIN_FLATTOP = gensym("flattop");
    WIN_GAUSS = gensym("gauss");
    WIN_DEFAULT = WIN_HANN;
    PROP_TYPE = gensym("@type");

    fn_map = {
        { WIN_TRIANGLE, window::triangle<t_float> },
        { WIN_GAUSS, window::gauss<t_float, 1> },
        { WIN_HANN, window::hann<t_float> },
        { WIN_RECT, window::rect<t_float> },
        { WIN_SINE, window::sine<t_float> },
        { WIN_HAMMING, window::hamming<t_float> },
        { WIN_BLACKMAN, window::blackman<t_float> },
        { WIN_NUTTALL, window::nuttall<t_float> },
        { WIN_BLACKMAN_HARRIS, window::blackman_harris<t_float> },
        { WIN_FLATTOP, window::flattop<t_float> },
        { WIN_WELCH, window::welch<t_float> }
    };

    ObjectFactory<ArrayWindow> obj("array.window");
    obj.addMethod("resize", &ArrayWindow::m_resize);
    obj.addMethod("fit", &ArrayWindow::m_fit);

    obj.addMethod("tri", &ArrayWindow::m_triangle);
    obj.addMethod("welch", &ArrayWindow::m_welch);
    obj.addMethod("hann", &ArrayWindow::m_hann);
    obj.addMethod("rect", &ArrayWindow::m_rect);
    obj.addMethod("sine", &ArrayWindow::m_sine);
    obj.addMethod("hamming", &ArrayWindow::m_hamming);
    obj.addMethod("blackman", &ArrayWindow::m_blackman);
    obj.addMethod("nuttall", &ArrayWindow::m_nuttall);
    obj.addMethod("blackman-harris", &ArrayWindow::m_blackman_harris);
    obj.addMethod("flattop", &ArrayWindow::m_flattop);
    obj.addMethod("gauss", &ArrayWindow::m_gauss);
}
