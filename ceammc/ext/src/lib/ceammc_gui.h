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

#include <string>

#include "cicm_wrapper.h"
#include "m_pd.h"

namespace ceammc_gui {

/** \deprecated this was a separate property handling, should be merged with existing CICM properties
     */
#pragma mark UI defines

#define UI_fun(x) template <> \
void ceammc_gui::GuiFactory<x>
#define UI_funp(x) template <> \
void* ceammc_gui::GuiFactory<x>
#define UI_METHOD_PTR(m) reinterpret_cast<method>(&GuiFactory<U>::m)

#pragma mark -

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
};

struct BaseSoundGuiStruct {
    t_edspbox j_box;

    // basic mouse handling
    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;
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
#pragma mark -

#pragma mark method 'extension' stubs

    /**
     * @brief "extension": bang method
     * @param z: pd object, s: symbol ("bang"), argc: argument count (should be 0), argv: argument value
     */
    static void m_bang(t_object* z)
    {
    }

    /**
     * @brief "extension": list method
     * @param z: pd object, s: symbol ("list"), argc: argument count , argv: argument value
     */
    static void m_list(t_object* z, t_symbol* s, int argc, t_atom* argv)
    {
    }

    /**
     * @brief "extension": anything method
     * @param z: pd object, s: symbol (message name, first element), argc: argument count, argv: argument value
     */
    static void m_anything(t_object* z, t_symbol* s, int argc, t_atom* argv)
    {
    }

    /**
     * @brief "extension": symbol method
     * @param z: pd object, s: symbol (the symbol itself), argc: argument count, argv: argument value
     */
    static void m_symbol(t_object* z, t_symbol* s)
    {
    }

    /**
     * @brief "extension": float method
     * @param z: pd object, f: float value
     */
    static void m_float(t_object* z, t_float f)
    {
    }

    /**
     * @brief "extension": set method
     * @param z: pd object, s: symbol ("set"), argc: argument count, argv: argument value
     */
    static void m_set(t_object* z, t_symbol* s, int argc, t_atom* argv)
    {
    }

    static void m_preset(t_object* z, t_binbuf *b)
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
    static void new_ext(t_object* z, t_symbol* s, int argc, t_atom* argv)
    {
    }

    /**
     * @brief DEPRECATED "extension": properties load routine
     * @param z: pd object
     */
    static void load_ext(t_object* z)
    {
    }

    /**
     * @brief "extension": pd object (instance) free method.
     * @param z: pd object
     */
    static void free_ext(t_object* z)
    {
    }

#pragma mark ui interaction 'extension' stubs

    /**
     * @brief "extension": mouse move event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousemove_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        GuiFactory<U>::ws_redraw(z);
    }

    /**
     * @brief "extension": mouse down event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousedown_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse up event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseup_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse drag event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousedrag_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse leave event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseleave_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": mouse leave event
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseenter_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
    }

    /**
     * @brief "extension": attribute update event
     * @param z: pd object, attr: attrobute name, attr_chage_type: type of change
     */
    static void wx_attr_changed_ext(t_object* /*z*/, t_symbol* /*attr*/)
    {
    }

    static void wx_mousewheel_ext(t_object* z, t_object* view, t_pt pt, long modifiers, double delta)
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

        t_binbuf* d = binbuf_via_atoms(argc, argv);
        ebox_new(asBox(z), 0 | EBOX_GROWINDI);

        if (z && d) {
            //moved
            new_ext(z, s, argc, argv);

            ebox_attrprocess_viabinbuf(z, d);
            ebox_ready(asBox(z));
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
        free_ext(z);
        ebox_free(asBox(z));
    }

    /**
     * @brief pd object dsp free method
     * @param z: pd object
     */
    static void free_dsp_method(t_object* x)
    {
        free_ext(x);
        eobj_dspfree(x);
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
    static void ws_redraw(t_object* z)
    {
        ebox_invalidate_layer(asBox(z), BG_LAYER);
        ebox_redraw(asBox(z));
    }

    /**
     * @brief CICM widget method: paint
     * @param z: pd object, view: canvas
     */
    static void wx_paint(t_object* z, t_object* view)
    {
        //float size;
        t_rect rect;
        ebox_get_rect_for_view(asBox(z), &rect);

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
    static void wx_oksize(t_object* z, t_rect* newrect)
    {
    }

#pragma mark extended - interaction

    /**
     * @brief CICM widget method: mouse move
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousemove(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        U* zx = reinterpret_cast<U*>(z);

        zx->mouse_x = pt.x;
        zx->mouse_y = pt.y;

        GuiFactory<U>::wx_mousemove_ext(z, view, pt, modifiers);
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

        wx_mousedown_ext(z, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse up
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseup(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        U* zx = reinterpret_cast<U*>(z);

        zx->mouse_x = pt.x;
        zx->mouse_y = pt.y;

        zx->mouse_dn = 0;

        GuiFactory<U>::wx_mouseup_ext(z, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse drag
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mousedrag(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        U* zx = reinterpret_cast<U*>(z);

        zx->mouse_x = pt.x;
        zx->mouse_y = pt.y;

        GuiFactory<U>::wx_mousedrag_ext(z, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse leave
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseleave(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        wx_mouseleave_ext(z, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: mouse enter
     * @param z: pd object, view: view, pt: mouse location, modifiers: modifiers
     */
    static void wx_mouseenter(t_object* z, t_object* view, t_pt pt, long modifiers)
    {
        wx_mouseenter_ext(z, view, pt, modifiers);
    }

    /**
     * @brief CICM widget method: notify on attr change
     * @param x: pd pbject
     */
    static t_pd_err wx_notify(t_object* x, t_symbol* s, t_symbol* msg, void*, void*)
    {
        if (msg == s_attr_modified)
            wx_attr_changed_ext(x, s);

        return 0;
    }

    static void wx_mousewheel(t_object* z, t_object* view, t_pt pt, long modifiers, double delta)
    {
        wx_mousewheel_ext(z, view, pt, modifiers, delta);
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
        eclass_addmethod(cl, UI_METHOD_PTR(m_preset),      "preset",   A_NULL,  0);

        eclass_addmethod(cl, UI_METHOD_PTR(m_set),         "set", A_GIMME, 0);
        // clang-format on
    }

    void setup_attributes(t_eclass* cl)
    {
        //hide standard CICM attributes
        CLASS_ATTR_INVISIBLE(cl, "fontname", 1);
        CLASS_ATTR_INVISIBLE(cl, "fontweight", 1);
        CLASS_ATTR_INVISIBLE(cl, "fontslant", 1);
        CLASS_ATTR_INVISIBLE(cl, "fontsize", 1);

        // background / border color
        CLASS_ATTR_LABEL(cl, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER(cl, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(cl, "bgcolor", 0, "0.75 0.75 0.75 1.");
        CLASS_ATTR_STYLE(cl, "bgcolor", 0, "color");

        // TODO
        // CLASS_ATTR_RGBA                 (cl, "bdcolor", 0, U, b_color_border);
        // CLASS_ATTR_LABEL                (cl, "bdcolor", 0, "Border Color");
        // CLASS_ATTR_ORDER                (cl, "bdcolor", 0, "2");
        // CLASS_ATTR_DEFAULT_SAVE_PAINT   (cl, "bdcolor", 0, "0.5 0.5 0.5 1.");
        // CLASS_ATTR_STYLE                (cl, "bdcolor", 0, "color");

        // default
        CLASS_ATTR_DEFAULT(cl, "size", 0, "45. 15.");
    }

    void do_setup(const std::string& cname, method new_method, method free_method, int flags, t_symbol* obj_class)
    {
        t_eclass* cl = eclass_new(cname.c_str(),
            new_method,
            free_method,
            sizeof(U), flags, A_GIMME, 0);
        if (cl) {
            eclass_guiinit(cl, 0);
            GuiFactory<U>::pd_class = cl;
            GuiFactory<U>::init_ext(cl);
            eclass_register(obj_class, cl);
        }
    }

    /**
     * @brief main setup routine
     * @details this methods creates new pd class with one inlet,
     * creates new default ui box attributes (CICM) then calls the 'init_ext' method
     * @param _class_name: the class name
     */
    void setup(const std::string& _class_name)
    {
        t_eclass* cl = eclass_new(_class_name.c_str(),
            UI_METHOD_PTR(new_method),
            UI_METHOD_PTR(free_method),
            sizeof(U), CLASS_PATCHABLE, A_GIMME, 0);

        if (cl) {
            eclass_guiinit(cl, 0);
            setup_methods(cl);
            setup_attributes(cl);
            GuiFactory<U>::pd_class = cl;
            GuiFactory<U>::init_ext(cl);
            eclass_register(CLASS_BOX, cl);
        }
    }

    /**
     * @brief GUI DSP setup routine
     * @details this methods creates new pd class with one inlet,
     * creates new default ui box attributes (CICM) then calls the 'init_ext' method
     * @param _class_name: the class name
     */
    void setup_dsp(const std::string& _class_name)
    {
        t_eclass* cl = eclass_new(_class_name.c_str(),
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
            eclass_register(CLASS_OBJ, cl);
        }
    }

    void setup_noin(const std::string& _class_name)
    {
        t_eclass* cl = eclass_new(_class_name.c_str(),
            UI_METHOD_PTR(new_method),
            UI_METHOD_PTR(free_method),
            sizeof(U), CLASS_NOINLET, A_GIMME, 0);

        if (cl) {
            eclass_guiinit(cl, 0);

            setup_methods(cl);
            setup_attributes(cl);
            GuiFactory<U>::pd_class = cl;
            GuiFactory<U>::init_ext(cl);
            eclass_register(CLASS_BOX, cl);
        }
    }

    /**
     * @brief temporary / stub
     * @param _class_name: the class name, inlets/outlets arrays
     */
    void setup_io(const std::string& _class_name, t_atomtype* ins, t_atomtype* outs)
    {
        this->setup(_class_name);
    }

    static U* asStruct(t_object* x)
    {
        return reinterpret_cast<U*>(x);
    }

    static t_ebox* asBox(t_object* x)
    {
        return reinterpret_cast<t_ebox*>(x);
    }
};

template <typename U>
t_eclass* GuiFactory<U>::pd_class = 0;

template <typename U>
t_symbol* GuiFactory<U>::BG_LAYER = gensym("background_layer");

template <typename U>
t_symbol* GuiFactory<U>::FONT_FAMILY = gensym("Helvetica");
template <typename U>
t_symbol* GuiFactory<U>::FONT_STYLE = gensym("roman");
template <typename U>
t_symbol* GuiFactory<U>::FONT_WEIGHT = gensym("normal");
template <typename U>
const int GuiFactory<U>::FONT_SIZE = 12;
template <typename U>
const int GuiFactory<U>::FONT_SIZE_SMALL = 8;
template <typename U>
t_symbol* GuiFactory<U>::COLOR_ACTIVE = gensym("#00C0FF");

}; //namespace ceammc_gui

#endif /* new_c_gui_h */
