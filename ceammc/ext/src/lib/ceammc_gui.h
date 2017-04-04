//
//  ceammc_gui.h
//
//  Created by Alex Nadzharov on 15/12/16.
//
//

/*
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

#ifndef new_c_gui_h
#define new_c_gui_h

#include <cassert>
#include <string>

#include "cicm_wrapper.h"
#include "m_pd.h"

namespace ceammc_gui {

//! Gettext extract message helper
#ifndef _
#define _(msg) msg
#endif

/** \deprecated this was a separate property handling, should be merged with existing CICM properties
     */
#pragma mark UI defines

#define UI_fun(x) template <> \
void ceammc_gui::GuiFactory<x>
#define UI_funp(x) template <> \
void* ceammc_gui::GuiFactory<x>
#define UI_METHOD_PTR(m) reinterpret_cast<method>(&GuiFactory<U>::m)

#pragma mark -

static const char* DEFAULT_ACTIVE_COLOR = "0. 0.75 1. 1.";
static const char* DEFAULT_BORDER_COLOR = "0.6 0.6 0.6 1.";
static const char* DEFAULT_BACKGROUND_COLOR = "0.93 0.93 0.93 1.";
static const char* DEFAULT_TEXT_COLOR = "0. 0. 0. 1.";

static const char* PROP_ACTIVE_COLOR = "active_color";
static const char* PROP_BACKGROUND_COLOR = "background_color";
static const char* PROP_BORDER_COLOR = "border_color";
static const char* PROP_TEXT_COLOR = "text_color";

#define HIDE_FONT_PROPS(cl) {\
    CLASS_ATTR_INVISIBLE(cl, "fontname", 1);\
    CLASS_ATTR_INVISIBLE(cl, "fontweight", 1);\
    CLASS_ATTR_INVISIBLE(cl, "fontslant", 1);\
    CLASS_ATTR_INVISIBLE(cl, "fontsize", 1);\
}

/**
 * @brief Structure prototype for pd object (t_object).
 */
struct BaseGuiObject {
    /** @brief CICM ui box.
     * this must be included in inherited structure 
     * @details for DSP object override with t_edspobj in inherited class (backwards compatible)
     */
    t_ebox b_box;

    // basic mouse handling

    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;

    t_rgba b_color_background;
    t_rgba b_color_border;

    // this was prototype for dynamic I/O
    // todo merge with CICM functionality

    //    t_inlet **inlets;
    //    t_inlet **outlets;
    //    int inlet_count;
    //    int oulet_count;
    //    t_atomtype *inlet_types;
    //    t_atomtype *outet_types;

    t_rect rect() const { return b_box.b_rect; }
    float x() const { return b_box.b_rect.x; }
    float y() const { return b_box.b_rect.y; }
    float width() const { return b_box.b_rect.width; }
    float height() const { return b_box.b_rect.height; }

    void send(t_float f)
    {
        t_pd* send = ebox_getsender(&b_box);
        if (send) {
            pd_float(send, f);
        }
    }

    void send(t_symbol* s)
    {
        t_pd* send = ebox_getsender(&b_box);
        if (send) {
            pd_symbol(send, s);
        }
    }

    void send(int argc, t_atom* argv)
    {
        t_pd* send = ebox_getsender(&b_box);
        if (send) {
            pd_list(send, &s_list, argc, argv);
        }
    }

    void send(t_symbol* s, int argc, t_atom* argv)
    {
        t_pd* send = ebox_getsender(&b_box);
        if (send) {
            pd_typedmess(send, s, argc, argv);
        }
    }

    void getRect(t_rect* r)
    {
        assert(r);
        ebox_get_rect_for_view(&b_box, r);
    }
};

struct BaseSoundGuiStruct {
    t_edspbox j_box;

    // basic mouse handling
    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;

    t_rgba b_color_background;
    t_rgba b_color_border;

    void getRect(t_rect* r)
    {
        assert(r);
        ebox_get_rect_for_view(reinterpret_cast<t_ebox*>(this), r);
    }
};

/**
 * @brief The template class for GUI objects. Provides basic functionality with CICM Wrapper.
 * @details To create your own UI object:
 *
 * 1. Create your structure for object. If you don't need any additional fields, just make struct new_obj:BaseGuiObject{};
 * 2. In the standard pd-object setup routine create new instance of GuiFactory<new_obj> and call setup()
 *
 * METHODS: All of the class methods here provide additional '[something]_ext' methods that can be used
 * if you want both standard action from this class and your custom code.
 * Otherwise just override the standard method by implicitly implementing object<your_object_struct_name>::method()
 */
template <typename U>
class GuiFactory {
    bool use_presets_;

public:
    GuiFactory()
        : use_presets_(false)
    {
    }

public:
#pragma mark static - definitions
    static t_eclass* pd_class;
    static t_symbol* BG_LAYER;
    static t_symbol* FONT_FAMILY;
    static t_symbol* FONT_STYLE;
    static t_symbol* FONT_WEIGHT;
    static t_symbol* COLOR_ACTIVE;
    static const int FONT_SIZE;
    static const int FONT_SIZE_SMALL;
    static long flags;
#pragma mark -

#pragma mark method 'extension' stubs

    /**
     * @brief "extension": bang method
     * @param z: pd object, s: symbol ("bang"), argc: argument count (should be 0), argv: argument value
     */
    static void m_bang(U* z)
    {
    }

    /**
     * @brief "extension": list method
     * @param z: pd object, s: symbol ("list"), argc: argument count , argv: argument value
     */
    static void m_list(U* z, t_symbol* s, int argc, t_atom* argv)
    {
    }

    /**
     * @brief "extension": anything method
     * @param z: pd object, s: symbol (message name, first element), argc: argument count, argv: argument value
     */
    static void m_anything(U* z, t_symbol* s, int argc, t_atom* argv)
    {
    }

    /**
     * @brief "extension": symbol method
     * @param z: pd object, s: symbol (the symbol itself), argc: argument count, argv: argument value
     */
    static void m_symbol(U* z, t_symbol* s)
    {
    }

    /**
     * @brief "extension": float method
     * @param z: pd object, f: float value
     */
    static void m_float(U* z, t_float f)
    {
    }

    /**
     * @brief "extension": set method
     * @param z: pd object, s: symbol ("set"), argc: argument count, argv: argument value
     */
    static void m_set(U* z, t_symbol* s, int argc, t_atom* argv)
    {
    }

    static void m_preset(U* z, t_binbuf* b)
    {
    }

#pragma mark basic 'extension' stubs

    /**
     * @brief "extension": class initialization
     * @param z: pd class
     */
    static void init_ext(t_eclass* z)
    {
    }

    /**
     * @brief "extension": pd object (instance) initialization
     * @param z: pd object, s: symbol ("set"), argc: argument count, argv: argument value
     */
    static void new_ext(U* z, t_symbol* s, int argc, t_atom* argv)
    {
    }

    /**
     * @brief "extension": pd object (instance) free method.
     * @param z: pd object
     */
    static void free_ext(U* z)
    {
    }

#pragma mark ui interaction 'extension' stubs

    /**
     * @brief "extension": mouse move event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousemove_ext(U* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse down event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousedown_ext(U* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse up event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseup_ext(U* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse drag event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousedrag_ext(U* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse leave event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseleave_ext(U* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse leave event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseenter_ext(U* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": attribute update event
     * @param z: pd object, attr: attrobute name, attr_chage_type: type of change
     */
    static void wx_attr_changed_ext(U* /*z*/, t_symbol* /*attr*/)
    {
    }

    static void wx_mousewheel_ext(U* z, t_object* view, t_pt pt, long modifiers, double delta)
    {
    }

    static void wx_ready(U*)
    {
    }

#pragma mark -
#pragma mark pd object instance

    /**
     * @brief new pd object instance.
     * @param s: symbol, argc: argument count, argv: argument value
     */
    static void* new_method(t_symbol* s, int argc, t_atom* argv)
    {
        t_object* z = reinterpret_cast<t_object*>(eobj_new(GuiFactory<U>::pd_class));
        t_ebox* box = reinterpret_cast<t_ebox*>(z);

        t_binbuf* d = binbuf_via_atoms(argc, argv);
        ebox_new(box, GuiFactory<U>::flags);

        if (z && d) {
            //moved
            new_ext(asStruct(z), s, argc, argv);

            ebox_attrprocess_viabinbuf(z, d);
            ebox_ready(box);
            wx_ready(asStruct(z));
            binbuf_free(d);
        }

        return static_cast<void*>(z);
    }

    /**
     * @brief pd object free method
     * @param z: pd object
     */
    static void free_method(t_object* z)
    {
        free_ext(asStruct(z));
        ebox_free(reinterpret_cast<t_ebox*>(z));
    }

    /**
     * @brief pd object dsp free method
     * @param z: pd object
     */
    static void free_dsp_method(t_object* z)
    {
        free_ext(asStruct(z));
        eobj_dspfree(z);
    }

#pragma mark -
#pragma mark standard widget methods

    /**
     * @brief pd widget method: select
     * @param z: pd graphic object, glist: pd canvas, selected: flag
     */
    static void w_select(t_gobj* z, t_glist* /*glist*/, int selected)
    {
        U* zx = reinterpret_cast<U*>(z);
        zx->_selected = selected;
    }

#pragma mark extended ui methods

    /**
     * @brief CICM widget method: redraw
     * @param z: pd object
     */
    static void ws_redraw(U* z)
    {
        ebox_invalidate_layer(asBox(z), BG_LAYER);
        ebox_redraw(asBox(z));
    }

    /**
     * @brief CICM widget method: paint
     * @param z: pd object, view: canvas
     */
    static void wx_paint(U* z, t_object* view)
    {
        //float size;
        t_rect rect;
        z->getRect(&rect);

        t_elayer* g = ebox_start_layer(asBox(z), BG_LAYER, rect.width, rect.height);
        if (g) {
            // EXAMPLE
            //            size = rect.width * 0.5;
            //            egraphics_set_color_hex(g, gensym("#00C0FF"));
            //            egraphics_circle(g, floor(size + 0.5), floor(size + 0.5), size * 0.9);
            //            egraphics_fill(g);
            ebox_end_layer(asBox(z), BG_LAYER);
        }

        ebox_paint_layer(asBox(z), BG_LAYER, 0., 0.);
    }

    /**
     * @brief CICM widget method: oksize
     * @details fix the object size in this method
     * @param z: pd object, newrect: object box rect
     */
    static void wx_oksize(U* z, t_rect* newrect)
    {
    }

#pragma mark extended - interaction

    /**
     * @brief CICM widget method: mouse move
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousemove(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        U* zx = asStruct(z);

        zx->mouse_x = pt.x;
        zx->mouse_y = pt.y;

        wx_mousemove_ext(zx, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse down
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousedown(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        U* zx = asStruct(z);

        zx->mouse_x = pt.x;
        zx->mouse_y = pt.y;

        zx->mouse_dn = 1;

        wx_mousedown_ext(zx, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse up
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseup(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        U* zx = asStruct(z);

        zx->mouse_x = pt.x;
        zx->mouse_y = pt.y;

        zx->mouse_dn = 0;

        wx_mouseup_ext(zx, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse drag
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousedrag(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        U* zx = asStruct(z);

        zx->mouse_x = pt.x;
        zx->mouse_y = pt.y;

        wx_mousedrag_ext(zx, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse leave
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseleave(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        wx_mouseleave_ext(asStruct(z), view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse enter
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseenter(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        wx_mouseenter_ext(asStruct(z), view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: notify on attr change
     * @param x: pd pbject
     */
    static t_pd_err wx_notify(t_object* z, t_symbol* s, t_symbol* msg, void*, void*)
    {
        if (msg == s_attr_modified)
            wx_attr_changed_ext(asStruct(z), s);

        return 0;
    }

    static void wx_mousewheel(t_object* z, t_object* view, t_pt pt, long modifiers, double delta)
    {
        wx_mousewheel_ext(asStruct(z), view, pt, modifiers, delta);
    }

#pragma mark -
#pragma mark setup

public:
    void setup_methods(t_eclass* cl)
    {
        // clang-format off
        eclass_addmethod(cl, UI_METHOD_PTR(wx_paint), "paint", A_GIMME, 0);

        eclass_addmethod(cl, UI_METHOD_PTR(wx_mousemove),  "mousemove", A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(wx_mousedown),  "mousedown", A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(wx_mouseup),    "mouseup",   A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(wx_mousedrag),  "mousedrag", A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(wx_mousewheel), "mousewheel", A_GIMME, 0);

        eclass_addmethod(cl, UI_METHOD_PTR(wx_mouseenter), "mouseenter", A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(wx_mouseleave), "mouseleave", A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(wx_notify),     "notify",     A_GIMME, 0);

        eclass_addmethod(cl, UI_METHOD_PTR(wx_oksize),     "oksize",   A_GIMME, 0);

        eclass_addmethod(cl, UI_METHOD_PTR(m_bang),        "bang",     A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(m_float),       "float",    A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(m_symbol),      "symbol",   A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(m_list),        "list",     A_GIMME, 0);
        eclass_addmethod(cl, UI_METHOD_PTR(m_anything),    "anything", A_GIMME, 0);

        eclass_addmethod(cl, UI_METHOD_PTR(m_set),         "set", A_GIMME, 0);

        if(use_presets_) {
            eclass_addmethod(cl, UI_METHOD_PTR(m_preset),   "preset", A_NULL, 0);
        }

        // clang-format on
    }

    void setup_attributes(t_eclass* cl)
    {
        // clang-format off
        //hide standard CICM attributes
        HIDE_FONT_PROPS(cl);

        // background / border color
        CLASS_ATTR_RGBA                 (cl, PROP_BACKGROUND_COLOR, 0, U, b_color_background);
        CLASS_ATTR_LABEL                (cl, PROP_BACKGROUND_COLOR, 0, _("Background Color"));
        CLASS_ATTR_ORDER                (cl, PROP_BACKGROUND_COLOR, 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (cl, PROP_BACKGROUND_COLOR, 0, DEFAULT_BACKGROUND_COLOR);
        CLASS_ATTR_STYLE                (cl, PROP_BACKGROUND_COLOR, 0, "color");

        CLASS_ATTR_RGBA                 (cl, PROP_BORDER_COLOR, 0, U, b_color_border);
        CLASS_ATTR_LABEL                (cl, PROP_BORDER_COLOR, 0, _("Border Color"));
        CLASS_ATTR_ORDER                (cl, PROP_BORDER_COLOR, 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (cl, PROP_BORDER_COLOR, 0, DEFAULT_BORDER_COLOR);
        CLASS_ATTR_STYLE                (cl, PROP_BORDER_COLOR, 0, "color");

        // default
        CLASS_ATTR_DEFAULT              (cl, "size", 0, "45. 15.");
        // clang-format on
    }

    /**
     * @brief main setup routine
     * @details this methods creates new pd class with one inlet,
     * creates new default ui box attributes (CICM) then calls the 'init_ext' method
     * @param _class_name: the class name
     */
    void setup(const char* class_name, long flags = EBOX_GROWINDI)
    {
        t_eclass* cl = eclass_new(class_name,
            UI_METHOD_PTR(new_method),
            UI_METHOD_PTR(free_method),
            sizeof(U), CLASS_PATCHABLE, A_GIMME, 0);

        if (cl) {
            eclass_guiinit(cl, 0);

            setup_methods(cl);
            setup_attributes(cl);
            GuiFactory<U>::pd_class = cl;
            GuiFactory<U>::init_ext(cl);
            GuiFactory<U>::flags = flags;
            eclass_register(CLASS_BOX, cl);
        }
    }

    /**
     * @brief GUI DSP setup routine
     * @details this methods creates new pd class with one inlet,
     * creates new default ui box attributes (CICM) then calls the 'init_ext' method
     * @param _class_name: the class name
     */
    void setup_dsp(const char* class_name, long flags = EBOX_GROWINDI)
    {
        t_eclass* cl = eclass_new(class_name,
            UI_METHOD_PTR(new_method),
            UI_METHOD_PTR(free_method),
            sizeof(U), CLASS_PATCHABLE, A_GIMME, 0);

        if (cl) {
            eclass_dspinit(cl);
            eclass_guiinit(cl, 0);

            setup_methods(cl);
            setup_attributes(cl);
            GuiFactory<U>::pd_class = cl;
            GuiFactory<U>::init_ext(cl);
            GuiFactory<U>::flags = flags;
            eclass_register(CLASS_OBJ, cl);
        }
    }

    void setup_noin(const char* class_name, long flags = EBOX_GROWINDI)
    {
        t_eclass* cl = eclass_new(class_name,
            UI_METHOD_PTR(new_method),
            UI_METHOD_PTR(free_method),
            sizeof(U), CLASS_NOINLET, A_GIMME, 0);

        if (cl) {
            eclass_guiinit(cl, 0);

            setup_methods(cl);
            setup_attributes(cl);
            GuiFactory<U>::pd_class = cl;
            GuiFactory<U>::init_ext(cl);
            GuiFactory<U>::flags = flags;
            eclass_register(CLASS_BOX, cl);
        }
    }

    void use_presets(bool v = true)
    {
        use_presets_ = v;
    }

    static U* asStruct(t_object* x)
    {
        return reinterpret_cast<U*>(x);
    }

    static t_ebox* asBox(U* x)
    {
        return reinterpret_cast<t_ebox*>(x);
    }

    static t_outlet* create_outlet(U* x, t_symbol* type = &s_anything)
    {
        return outlet_new(reinterpret_cast<t_object*>(x), type);
    }
};

template <typename U>
long GuiFactory<U>::flags = 0;

template <typename U>
t_eclass* GuiFactory<U>::pd_class = 0;

template <typename U>
t_symbol* GuiFactory<U>::BG_LAYER = gensym("background_layer");

#ifdef __APPLE__
template <typename U>
t_symbol* GuiFactory<U>::FONT_FAMILY = gensym("Helvetica");
template <typename U>
const int GuiFactory<U>::FONT_SIZE = 12;
template <typename U>
const int GuiFactory<U>::FONT_SIZE_SMALL = 8;
#elif _WIN32
template <typename U>
t_symbol* GuiFactory<U>::FONT_FAMILY = gensym("Verdana");
template <typename U>
const int GuiFactory<U>::FONT_SIZE = 9;
template <typename U>
const int GuiFactory<U>::FONT_SIZE_SMALL = 6;
#else
template <typename U>
t_symbol* GuiFactory<U>::FONT_FAMILY = gensym("OpenSans");
template <typename U>
const int GuiFactory<U>::FONT_SIZE = 10;
template <typename U>
const int GuiFactory<U>::FONT_SIZE_SMALL = 6;
#endif

template <typename U>
t_symbol* GuiFactory<U>::FONT_STYLE = gensym("roman");
template <typename U>
t_symbol* GuiFactory<U>::FONT_WEIGHT = gensym("normal");
template <typename U>
t_symbol* GuiFactory<U>::COLOR_ACTIVE = gensym("#00C0FF");

}; //namespace ceammc_gui

#endif /* new_c_gui_h */
