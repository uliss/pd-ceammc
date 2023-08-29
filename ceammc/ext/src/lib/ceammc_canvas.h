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
#include "ceammc_geom.h"
#include "ceammc_pd.h"

#include <functional>
#include <iosfwd>
#include <map>
#include <memory>
#include <string>

struct _glist;
struct _text;
struct _canvasenvironment;
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
 * Returns parent canvas for given
 * @param c - pointer to canvas
 * @param level - netsted level, 0 for root canvas, >0 for subpatches
 * @param breakOnAbs - if true: break on abstraction top level
 * @return NULL on error
 */
const _glist* canvas_root(const _glist* c, int& level, bool breakOnAbs);

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

/**
 * Return canvas zoom or 0 on error
 */
int canvas_info_zoom(const _glist* c);

AtomList canvas_info_paths(const _glist* c);
AtomListView canvas_info_args(const _glist* c);

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

int canvas_info_dollarzero(const _glist* c);

/**
 * mark/unmark canvas dirty (show changes in canvas window)
 * @param c - canvas pointer
 * @param value
 */
void canvas_mark_dirty(_glist* c, bool value = true);

void canvas_set_current(const _glist* c);
void canvas_unset_current(const _glist* c);
_canvasenvironment* canvas_get_env(const _glist* c);
t_symbol* canvas_expand_dollar(const _glist* c, t_symbol* s, bool check);
void canvas_send_bang(_glist* c);

/**
 * Returns canvas rect, for root canvas and abstractions - window rect, for others - GOP
 * @param c - pointer to canvas
 */
t_rect canvas_info_rect(const _glist* c);

/**
 * Returns GOP (graph-on-parent) rectangle
 * @param c - pointer to canvas
 */
t_rect canvas_info_gop_rect(const _glist* c);

using CanvasClassPredicate = std::function<bool(const _glist*, const t_object*)>;
/**
 * Returns canvas object tree
 * @param c - pointer to canvas
 * @return tree
 */
std::unique_ptr<pd::CanvasTree> canvas_info_tree(const _glist* c, CanvasClassPredicate pred = {});

/**
 * Returns canvas object satisfied to predicate
 * @param c - pointer to canvas
 * @return vector of pointers
 */
std::vector<t_object*> canvas_find(const _glist* c, CanvasClassPredicate pred);

/**
 * iterate over all canvas items
 */
void canvas_foreach(const _glist* c, std::function<void(t_gobj*, const t_class*)> fn);

/**
 * find canvas last object
 */
t_gobj* canvas_find_last(const _glist* c);

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

    void createPdObject(int x, int y, t_symbol* name, const AtomListView& args = {});
    _glist* createAbstraction(int x, int y, t_symbol* name, const AtomListView& args = {});

    void loadBang();
    void show();
    void hide();
    void free();
    void setupDsp();

    operator bool() { return canvas_ != nullptr; }

public:
    _glist* pd_canvas() { return canvas_; }
    _glist* owner();
    t_pd* pd();

    /**
     * canvas name
     * @return canvas name or &s_ on null canvas
     */
    t_symbol* name();
    void setName(const char* str);
    const char* parentName() const;

public:
    static _glist* current();
    static void setCurrent(_glist* c);
};

std::ostream& operator<<(std::ostream& os, const Canvas& cnv);
}

#endif // CEAMMC_CANVAS_H
