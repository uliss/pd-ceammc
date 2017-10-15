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
#ifndef CEAMMC_CANVAS_H
#define CEAMMC_CANVAS_H

#include "ceammc_array.h"
#include "ceammc_atomlist.h"

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

struct _glist;
struct _symbol;
struct _text;
typedef struct _text t_object;

namespace ceammc {

/**
 * Checks if given canvas is top level (window)
 * @param c - pointer to canvas
 */
bool canvas_info_is_root(const _glist* c);

/**
 * Checks if given canvas is abstraction
 * @param c - pointer to canvas
 */
bool canvas_info_is_abstraction(const _glist* c);

AtomList canvas_info_paths(const _glist* c);
AtomList canvas_info_args(const _glist* c);

/**
 * Returns canvas name or empty symbol (not NULL!) on error
 * @c - pointer to canvas
 */
t_symbol* canvas_info_name(const _glist* c);

/**
 * Returns canvas directory or empty symbol (not NULL!) on error
 * @param c - pointer to canvas
 */
t_symbol* canvas_info_dir(const _glist* c);

/**
 * Returns canvas font size
 * @param c - pointer to canvas
 * @return font size or 0 on error
 */
int canvas_info_font(const _glist* c);

struct t_rect {
    int x, y, w, h;
    t_rect(int x_, int y_, int w_, int h_)
        : x(x_)
        , y(y_)
        , w(w_)
        , h(h_)
    {
    }

    bool operator==(const t_rect& r) const { return x == r.x && y == r.y && w == r.w && h == r.h; }
};

/**
 * Returns canvas rect, for root canvas - windows, for others - GOP
 * @param c - pointer to canvas
 */
t_rect canvas_info_rect(const t_canvas* c);

class BaseObject;
typedef boost::shared_ptr<Array> ArrayPtr;
typedef std::map<_symbol*, ArrayPtr> ArrayMap;

class Canvas {
    ArrayMap array_list_;
    _glist* canvas_;

public:
    Canvas(_glist* t);

    ArrayPtr createArray(const char* name, size_t n);

    static bool connect(t_object* src, size_t nout, t_object* dest, size_t ninl);
    static bool connect(const BaseObject& src, size_t nout, BaseObject& dest, size_t ninl);

public:
    _glist* pd_canvas() { return canvas_; }
    _glist* owner();
    t_symbol* name();
    void setName(const char* str);
    std::string parentName() const;

public:
    static _glist* current();
    static void setCurrent(_glist* c);
};
}

#endif // CEAMMC_CANVAS_H
