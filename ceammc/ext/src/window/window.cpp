/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "window.h"
#include "ceammc_factory.h"
#include "ceammc_window.h"

#include <algorithm>
#include <map>

static t_symbol* WIN_TRIANGLE = gensym("tri");
static t_symbol* WIN_WELCH = gensym("welch");
static t_symbol* WIN_HANN = gensym("hann");
static t_symbol* WIN_DEFAULT = WIN_HANN;
static const size_t DEFAULT_SIZE = 512;

typedef std::map<t_symbol*, WindowFuncPtr> WFuncMap;
static WFuncMap win_func_map;

static bool initFuncMap()
{
    win_func_map[WIN_DEFAULT] = window::triangle<float>;
    win_func_map[WIN_WELCH] = window::welch<float>;
    win_func_map[WIN_TRIANGLE] = window::triangle<float>;
    win_func_map[WIN_HANN] = window::hann<float>;
    return true;
}

namespace {
bool init = initFuncMap();
}

Window::Window(const PdArgs& a)
    : BaseObject(a)
    , size_(0)
    , type_(WIN_DEFAULT)
    , fn_(0)
{
    createCbProperty("@type", &Window::pTypeGet, &Window::pTypeSet);
    setWindowFunc(positionalSymbolArgument(0, WIN_DEFAULT));

    int arg_size = int(positionalFloatArgument(1, DEFAULT_SIZE));
    if (arg_size < 1)
        arg_size = DEFAULT_SIZE;

    size_ = new SizeTProperty("@size", size_t(arg_size));
    createProperty(size_);

    createOutlet();
}

void Window::onFloat(float v)
{
    int idx = int(v);
    float res = 0;

    if (0 <= idx && size_t(idx) < size_->value())
        res = fn_(size_t(idx), size_->value());

    floatTo(0, res);
}

void Window::onList(const AtomList& l)
{
    if (!checkArgs(l, ARG_INT, ARG_INT))
        return;

    size_->set(l[1]);
    onFloat(l[0].asFloat());
}

void Window::pTypeSet(const AtomList& l)
{
    if (!checkArgs(l, ARG_SYMBOL))
        return;

    setWindowFunc(l[0].asSymbol());
}

AtomList Window::pTypeGet() const
{
    return AtomList(type_);
}

void Window::setWindowFunc(t_symbol* name)
{
    WFuncMap::iterator it = win_func_map.find(name);
    if (it == win_func_map.end()) {
        OBJ_ERR << "unknown window type: " << name->s_name << ". setting default: "
                << WIN_DEFAULT->s_name;
        return setWindowFunc(WIN_DEFAULT);
    }

    fn_ = it->second;
    type_ = it->first;
}

WindowFuncPtr Window::windowFunc()
{
    return fn_;
}

void Window::m_hann(t_symbol*, const AtomList&)
{
    setWindowFunc(WIN_HANN);
}

void Window::m_tri(t_symbol*, const AtomList&)
{
    setWindowFunc(WIN_TRIANGLE);
}

void Window::m_welch(t_symbol*, const AtomList&)
{
    setWindowFunc(WIN_WELCH);
}

void window_setup()
{
    ObjectFactory<Window> obj("window");
    obj.addAlias("win");
    obj.addMethod("tri", &Window::m_tri);
    obj.addMethod("welch", &Window::m_welch);
    obj.addMethod("hann", &Window::m_hann);
}
