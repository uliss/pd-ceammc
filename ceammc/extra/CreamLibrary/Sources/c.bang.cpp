/*
 * Cream Library
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CreamLibrary
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/*!
 * \example c.bang.cpp
 * \brief This is an example of how to use and create a basic GUI.
 * \details This example shows how to initialize a GUI class with methods for painting and mouse interractions and attributes.
 */

//
// c.bang.cpp
// This is an example of how to use and create a basic GUI.
// This example shows how to initialize a GUI class with methods for painting and mouse interractions and attributes.
//

// This header includes cicm_wrapped.h
#include "../c.library.hpp"

/*! @addtogroup groupbang The Bang Example Part
 * @brief The example part for t_ebox.
 * @details This part is an example shows how to initialize a GUI class with methods for painting and mouse interractions and attributes.
 *  @{
 */

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////              STRUCTURE               ////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

//
// The GUI bang structure.
// It is a basic GUI struture with attributes and basic Pd stuffs.
//
/**
 * @struct t_bang
 * @brief The GUI bang structure.
 * @details It is a basic GUI struture with attributes and basic Pd stuffs.
 */
typedef struct t_bang
{
    // The t_ebox that allows to create a GUI.
	t_ebox      b_box;              /*!< The t_ebox that allows to create a GUI. */
    // The t_outlet of the object.
    t_outlet*   b_out;              /*!< The t_outlet of the object. */
    // The struture for the t_eattr background color.
    t_rgba		color_background; /*!< The struture for the t_eattr background color. */
    // The struture for the t_eattr border color.
    t_rgba		color_border;     /*!< The struture for the t_eattr border color. */
    // The struture for the t_eattr bang color.
    t_rgba		color_active;       /*!< The struture for the t_eattr bang color. */
    // The t_clock of the object.
    t_clock*    b_clock;            /*!< The t_clock of the object. */
    // If the object is performming a bang.
    char        b_active;           /*!< If the object is performming a bang. */   
} t_bang;

//! @cond
static void *bang_new(t_symbol *s, int argc, t_atom *argv);
static void bang_free(t_bang *x);
static void bang_activate(t_bang *x);
static void bang_deactivate(t_bang *x);
static void bang_anything(t_bang *x, t_symbol *s, int argc, t_atom *argv);
static void bang_getdrawparams(t_bang *x, t_object *view, t_edrawparams *params);
static void bang_oksize(t_bang *x, t_rect *newrect);
static void bang_paint(t_bang *x, t_object *view);
static void bang_mousedown(t_bang *x, t_object *view, t_pt pt, long modifiers);
static void bang_mouseup(t_bang *x, t_object *view, t_pt pt, long modifiers);
//! @endcond

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////        CLASS INITIALIZATION          ////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

// The t_eclass for the t_bang.
/*! The t_eclass for the t_bang. */
static t_eclass *bang_class;

// The "background_layer" t_symbol.
/*! The \"background_layer\" t_symbol. */
static t_symbol* bang_sym_background_layer;

// Setups the bang_class for GUI behavior.
// This function is called by Pd to initialize the bang_class of the c.bang object. In the function,
// you should set up the default methods, behaviors and attributes of your object. If your object's name is simple like
// "bang", this function should be named "bang_setup" otherwise, if your object's name is more complex like "c.bang" the
// function's name should replace the special character with the matching UTF8-hexadecimal value and put "setup" before the name
// of the object. In this example, the dot is replaced with "0x2e" and the function name becomes "setup_ui0x2ebang".
/*!
 * \fn          extern "C" void setup_ui0x2ebang(void)
 * \brief       Setups the bang_class for GUI behavior.
 * \details     This function is called by Pd to initialize the bang_class of the c.bang object. In the function, you should set up the default methods, behaviors and attributes of your object. If your object's name is simple like \"bang\", this function should be named \"bang_setup\" otherwise, if your object's name is more complex like \"c.bang\" the function's name should replace the special character with the matching UTF8-hexadecimal value and put \"setup\" before the name of the object. In this example, the dot is replaced with \"c0x2e\" and the function name becomes \"setup_ui0x2ebang\".
 */
extern "C" void setup_ui0x2ebang(void)
{
    // We creates a new t_eclass for the t_bang structure.
    // You should use this method for all the objects that uses either the t_eobj, t_edspobj, t_ebox or t_edspbox structure.
    // First argument is the name of the object. Second argument is the new method of the object. Third argument is the free method
    // of the object (if you only have to call eobj_free, ebox_free or eobj_dspfree you can directly use this methods. Fourth
    // argument is the size of the structure. Fifth argument is the type of the object (you should use CLASS_DEFAULT for every
    // patchable object or CLASS_NOINLET if you don't want a default first inlet, CLASS_PD should be used for no patchable object).
    // Sixth argument is the kind parameters you creation method is excepting (for GUI, you should always use A_GIMME to be able
    // to parse attributes. The last argument is dummy and should always be zero for the moment.
    t_eclass *c = eclass_new("ui.bang", (method)bang_new, (method)bang_free, (short)sizeof(t_bang), CLASS_DEFAULT, A_GIMME, 0);
    if(c)
    {
        // We initialize the defaults attributes and methods for the GUI.
        eclass_guiinit(c, 0);
        
        // We initialize of the specifics methods that the t_bang can receive. The "paint", "getdrawparams" and "oksize" names
        // are for the graphical methods ("getdrawparams" and "oksize" are optional but facilitates the setting of GUI.
        // The "mousedown" and "mouseup" are for the mouse interraction.
        // The other are the default message that a native Pd object can receive through an inlet.
        eclass_addmethod(c, (method) bang_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) bang_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) bang_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) bang_anything,        "float",            A_FLOAT,0);
        eclass_addmethod(c, (method) bang_anything,        "bang",             A_NULL, 0);
        eclass_addmethod(c, (method) bang_anything,        "list",             A_GIMME,0);
        eclass_addmethod(c, (method) bang_anything,        "anything",         A_GIMME,0);
        eclass_addmethod(c, (method) bang_mousedown,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) bang_mouseup,         "mouseup",          A_NULL, 0);
        
        // We intialize the attribute of the t_bang.
        // All the GUI classes has font attributes but we don't need them for the bang classe so we mark them invisible.
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        // All the GUI classes has a size attribute, we just set up the default value.
        CLASS_ATTR_DEFAULT              (c, "size", 0, "16 16");

        ATTR_DEFAULT_COLOR_BORDER       (c, t_bang);
        ATTR_DEFAULT_COLOR_BACKGROUND   (c, t_bang);
        ATTR_DEFAULT_COLOR_ACTIVE       (c, t_bang);
        
        // We register the class. This function is important it will set up some dsp members if needs and the properties window
        // if the class has attributes. The CLASS_BOX is for GUI otherwise use CLASS_OBJ this is pretty useless but it ensures
        // compatibilty with Max.
        eclass_register(CLASS_BOX, c);
        
        // We initialize bang_class with c. This is important because we use the adress of bang_class in the new method.
        bang_class = c;
        // We save the adress of the "background_layer" symbol to avoid the multiple call of the function.
        bang_sym_background_layer = gensym("background_layer");
    }
}

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////     OBJECT CREATION AND DELETION     ////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

// Allocates and initializes the t_bang structure.
// The function uses eobj_new to allocate the t_bang with the bang_class and initializes the default values.
/*!
 * \fn          static void *bang_new(t_symbol *s, int argc, t_atom *argv)
 * \brief       Allocates and initializes the t_bang structure.
 * \details     The function uses eobj_new to allocate the t_bang with the bang_class and initializes the default values.
 * \param s     The name of the object (in this case \"c.bang\").
 * \param argc  The number of t_atom parameters.
 * \param argv  The array of t_atom parameters.
 * \return      The pointer to the t_bang if the allocation has been successful, otherwise NULL.
 */
static void *bang_new(t_symbol *s, int argc, t_atom *argv)
{
    // We calls eobj_new with the bang_class to allocate the memory for the t_bang and to initialize some default values
    // and methods.
    t_bang *x  = (t_bang *)eobj_new(bang_class);
    // We transform the array of t_atoms into a t_binbuf. You can alos use directly the array of t_atoms but you should prefer
    // to use a t_binbuf to increases the compatibilty with Max that use the it's counterpart t_dictionnary.
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    if(x && d)
    {
        // We initialize the defaults members of the t_ebox. EBOX_GROWLINK mark that the width and the height of the
        // object are linked. The method also initializes the attributes with their defaults values.
        ebox_new((t_ebox *)x, 0 | EBOX_GROWLINK);
        // We allocate and intialize a new outlet that we send only bang message.
        x->b_out = outlet_new((t_object *)x, &s_bang);
        // We sets that the t_bang is currently inactive.
        x->b_active = 0;
        // We initialize a new t_clock to make the object blink few ms when it received a message. We usually define a
        // specific tick_method but here we can use the bang_deactivate method.
        x->b_clock          = clock_new(x,(t_method)bang_deactivate);
        // We parse the t_binbuf to initialize the values of the attributes. You can also use ebox_attrprocess_viatoms but you
        // should prefer this method.
        ebox_attrprocess_viabinbuf(x, d);
        // Indicates that the t_ebox can be drawn.
        ebox_ready((t_ebox *)x);

        binbuf_free(d);
    }
     // Returns the t_bang
    return (x);
}

// Frees the t_bang structure.
// The function just calls ebox_free() and frees the t_clock.
/*!
 * \fn          static void bang_free(t_bang *x)
 * \brief       Frees the t_bang structure.
 * \details     The function just calls ebox_free() and frees the t_clock.
 * \param x     The t_bang pointer.
 */
static void bang_free(t_bang *x)
{
    // We free the t_ebox, it will detach the graphical interface from Tcl/Tk
    ebox_free((t_ebox *)x);
    // We free the t_clock
    clock_free(x->b_clock);
}

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////       OBJECT GRAPHICAL METHODS       ////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

// Defines the default graphical parameters of a GUI.
// The function is used to defines the border size, the border size (dummy mostly for Max compatibility)
// the border color and the background color of a t_ebox.
/*!
 * \fn          static void bang_getdrawparams(t_bang *x, t_object *view, t_edrawparams *params)
 * \brief       Defines the default graphical parameters of a GUI.
 * \details     The function is used to defines the border size, the border size (dummy mostly for Max compatibility) the border color and the background color of a t_ebox.
 * \param x         The t_bang pointer.
 * \param view      The view pointer (dummy).
 * \param params    The t_edrawparams structure to set up.
 */
static void bang_getdrawparams(t_bang *x, t_object *view, t_edrawparams *params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

// Defines and validates the size of a GUI.
// The function is validate the new size of a GUI. It can be used to restrict the size.
/*!
 * \fn          static void bang_oksize(t_bang *x, t_rect *newrect)
 * \brief       Defines and validates the size of a GUI.
 * \details     The function is validate the new size of a GUI. It can be used to restrict the size.
 * \param x         The t_bang pointer.
 * \param newrect   The t_rect pointer to change if needed.
 */
static void bang_oksize(t_bang *x, t_rect *newrect)
{
    // We defines a minimum height and width of 15 px.
    newrect->width = pd_clip_min(newrect->width, 10.);
    newrect->height = pd_clip_min(newrect->height, 10.);
    // We defines that the width and the height can't be an even number (to center the bang circle).
    if((int)newrect->width % 2 == 0)
        newrect->width++;
    if((int)newrect->height % 2 == 0)
        newrect->height++;
}

// Paints the t_ebox.
// The function is called internally when the t_ebox should be repainted. This is where you should draw all the stuffs
// you want.
/*!
 * \fn          static void bang_paint(t_bang *x, t_object *view)
 * \brief       Paints the t_ebox.
 * \details     The function is called internally when the t_ebox should be repainted. This is where you should draw all the stuffs
 you want.
 * \param x      The t_bang pointer.
 * \param view   The view that ask to be repainted (dummy mostly for Max compatibility).
 */
static void bang_paint(t_bang *x, t_object *view)
{
    t_rect rect;
    // We defines a initialize a t_rect with the size of the t_ebox.
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    // We ask to retrieves the background t_elayer with a that has the size of the t_ebox. This layer will be binded to the t_ebox
    // with the t_symbol bang_sym_background_layer aka \"background_layer\". You should always prefer to use static t_symbol to
    // avoid to call gensym function.
    t_elayer *g = ebox_start_layer((t_ebox *)x, bang_sym_background_layer, rect.width, rect.height);
    // If it is a new t_elayer or if the layer has been ivalidated we can draw something in it, otherwise the pointer is NULL.
    if(g)
    {
        const int box_center = static_cast<int>(floorf(rect.width * 0.5f));
        int circle_radius = static_cast<int>(roundf(box_center * 0.9f));

        // fix for small sizes
        if(rect.width < 20)
            circle_radius = box_center;

        // align fix
        if((box_center - circle_radius) % 2 == 1)
            circle_radius--;

        // We set up the bang color is the t_bang is currently active.
        // Otherwise we use the background color.
        
        // We add a circle at the center the t_elayer.
        egraphics_circle(g, box_center, box_center, circle_radius);
        
        if(x->b_active)
        {
            egraphics_set_color_rgba(g, &x->color_active);
            // We fill the t_elayer with the drawing.
            egraphics_fill(g);
        }
        else
        {
            egraphics_set_color_rgba(g, &x->color_border);
            // We stroke the t_elayer with the drawing.
            egraphics_stroke(g);
        }
        // We mark the layer as ready to be painted.
        ebox_end_layer((t_ebox*)x, bang_sym_background_layer);
    }
    // We tell the t_ebox to painted the \"background_layer\" layer at the 0 0 position. If the t_elayer was invalid or new
    // that means that we have drawn something new in it, otherwise t_ebox paint the t_elayer like it was before.
    ebox_paint_layer((t_ebox *)x, bang_sym_background_layer, 0., 0.);
}

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////      OBJECT MOUSE INTERRACTIONS      ////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

// Activates the status of the t_bang.
// The function activates the status of the t_bang. Outputs a bang message, sends a bang to the attached objects and ask the t_ebox
// to be redrawn
/*!
 * \fn          static void bang_activate(t_bang *x)
 * \brief       Activates the status of the t_bang.
 * \details     The function activates the status of the t_bang. Outputs a bang message, sends a bang to the attached objects and ask the t_ebox to be redrawn
 * \param x      The t_bang pointer.
 */
static void bang_activate(t_bang *x)
{
    // We mark the bang as active.
    x->b_active = 1;
    // We send a bang through our outlet.
    outlet_bang(x->b_out);
    
    // We retrieves the link list of object attached to the t_bang.
    t_pd* senders = ebox_getsender((t_ebox *) x);
    if(senders)
    {
        // If we have a link list we send a bang to the object.
        // All the GUI has send and receive t_symbol attributes to send and receive messages without connections.
        pd_bang(senders);
    }
    
    // We invalidate the background layer.
    ebox_invalidate_layer((t_ebox *)x, bang_sym_background_layer);
    // We aks the t_ebox to redraw the object.
    ebox_redraw((t_ebox *)x);
}

// Deactivates the status of the t_bang.
// The function deactiveates the status of the t_bang and ask the t_ebox to be redrawn
/*!
 * \fn          static void bang_deactivate(t_bang *x)
 * \brief       Deactivates the status of the t_bang.
 * \details     The function deactiveates the status of the t_bang and ask the t_ebox to be redrawn
 * \param x      The t_bang pointer.
 */
static void bang_deactivate(t_bang *x)
{
    // We mark the bang as inactive.
    x->b_active = 0;
    // We invalidate the background layer.
    ebox_invalidate_layer((t_ebox *)x, bang_sym_background_layer);
    // We aks the t_ebox to redraw the object.
    ebox_redraw((t_ebox *)x);
}

// Receives the mouse down notification..
// The function is called internally when the object has been clicked.
/*!
 * \fn          static void bang_mousedown(t_bang *x, t_object *view, t_pt pt, long modifiers)
 * \brief       Receives the mouse down notification.
 * \details     The function is called internally when the object has been clicked.
 * \param x     The t_bang pointer.
 * \param view   The view (dummy mostly for Max compatibility).
 * \param pt   The position of the mouse within the object.
 * \param modifiers  The modifiers (alt, ctrl, etc.).
 */
static void bang_mousedown(t_bang *x, t_object *view, t_pt pt, long modifiers)
{
    // In this example, when the mouse down is called we activate the t_bang.
    bang_activate(x);
}

// Receives the mouse up notification..
// The function is called internally when the mouse has been released from the object.
/*!
 * \fn          static void bang_mouseup(t_bang *x, t_object *view, t_pt pt, long modifiers)
 * \brief       Receives the mouse up notification.
 * \details     The function is called internally when the mouse has been released from the object.
 * \param x     The t_bang pointer.
 * \param view   The view (dummy mostly for Max compatibility).
 * \param pt   The position of the mouse within the object.
 * \param modifiers  The modifiers (alt, ctrl, etc.).
 */
static void bang_mouseup(t_bang *x, t_object *view, t_pt pt, long modifiers)
{
    // In this example, when the mouse down is called we deactivate the t_bang.
    bang_deactivate(x);
}


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////      OBJECT MESSAGES RECEPTION       ////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

// Receives anything.
// The function is called when any message has been received from the inlet.
/*!
 * \fn          static void bang_anything(t_bang *x, t_symbol *s, int argc, t_atom *argv)
 * \brief       Receives the anything notification.
 * \details     The function is called when any message has been received from the inlet.
 * \param x     The t_bang pointer.
 * \param s     The message symbol.
 * \param argc  The number of t_atom.
 * \param argv  The array of t_atom.
 */
static void bang_anything(t_bang *x, t_symbol *s, int argc, t_atom *argv)
{
    // When we receive any message we activate the t_bang
    bang_activate(x);
    // then delay the clock of 100ms to deactivate the t_bang
    clock_delay(x->b_clock, 100);
    // thus the object will blink.
}

/** @} */





