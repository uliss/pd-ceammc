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
#include "ceammc_pd.h"

#include <functional>
#include <map>
#include <memory>
#include <string>

struct _glist;
struct _text;
typedef struct _text t_object;

namespace ceammc {

namespace pd {
    class External;

    class CanvasTree {
    public:
        t_symbol* cnv_name;
        t_symbol* cnv_dir;

        enum Type {
            OBJECT,
            SUBPATCH,
            ABSTRACTION
        };

        struct Object {
            t_symbol* obj_name;
            t_symbol* obj_dir;
            std::unique_ptr<CanvasTree> obj_tree;
            Type obj_type;

            Object(t_symbol* name, t_symbol* dir, Type t);
            Object(Object&& obj);
            Object& operator=(Object&& obj);
        };

        std::vector<Object> objects;

        CanvasTree(t_symbol* name, t_symbol* dir);
        CanvasTree(CanvasTree&& t);
        CanvasTree& operator=(CanvasTree&& t);
        void appendObj(Object&& obj) { objects.push_back(std::move(obj)); }
    };
}

/**
 * Checks if given canvas is top level (window)
 * @param c - pointer to canvas
 */
bool canvas_info_is_root(const _glist* c);

/**
 * Returns parent canvas for given
 * @param c - pointer to canvas
 * @return NULL on error
 */
const _glist* canvas_root(const _glist* c);

/**
 * Checks if given canvas is abstraction
 * @param c - pointer to canvas
 */
bool canvas_info_is_abstraction(const _glist* c);

/**
 * Checks is canvas is modified
 * @param c - pointer to root canvas
 */
bool canvas_info_is_dirty(const _glist* c);

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
t_rect canvas_info_rect(const _glist* c);

/**
 * Returns canvas object tree
 * @param c - pointer to canvas
 * @return tree
 */
std::unique_ptr<pd::CanvasTree> canvas_info_tree(const _glist* c);

class BaseObject;
typedef std::shared_ptr<Array> ArrayPtr;

class Canvas {
    _glist* canvas_;

public:
    Canvas(_glist* t);
    ~Canvas();

    ArrayPtr createArray(const char* name, size_t n);

    static bool connect(t_object* src, size_t nout, t_object* dest, size_t ninl);
    static bool connect(const BaseObject& src, size_t nout, BaseObject& dest, size_t ninl);

    std::vector<const t_object*> objectList() const;
    std::vector<const t_object*> findObjectByClassName(t_symbol* name);
    t_gobj* findIf(std::function<bool(t_gobj*)> pred);
    t_object* findIf(std::function<bool(t_object*)> pred);

    void addExternal(pd::External& ext);
    std::shared_ptr<pd::External> createObject(const char* name, const AtomList& args);

    void createPdObject(int x, int y, t_symbol* name, const AtomList& args = AtomList());
    _glist* createAbstraction(int x, int y, t_symbol* name, const AtomList& args = AtomList());

    void loadBang();
    void show();
    void hide();
    void free();
    void setupDsp();

    operator bool() { return canvas_ != nullptr; }

public:
    _glist* pd_canvas() { return canvas_; }
    _glist* owner();

    /**
     * canvas name
     * @return canvas name or &s_ on null canvas
     */
    t_symbol* name();
    void setName(const char* str);
    std::string parentName() const;

public:
    static _glist* current();
    static void setCurrent(_glist* c);
};
}

#endif // CEAMMC_CANVAS_H
