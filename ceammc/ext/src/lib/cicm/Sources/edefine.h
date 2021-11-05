/*
 * CicmWrapper - Pure Data Enhanced

 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 * @file edefine.h
 * @brief The header that contains all the principals structures of CicmWrapper
 * @details All the principals structures are defined in this file.
 * @author Pierre Guillot
 */

#ifndef DEF_ESTRUCT
#define DEF_ESTRUCT

#ifdef _WINDOWS
#include <io.h>
#define _FUNCTION_DEPRECTAED_
#else
#include <unistd.h>
#define _FUNCTION_DEPRECTAED_ __attribute__((deprecated))
#endif

#include "m_pd.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#ifdef _MSC_VER

#define snprintf sprintf_s
#endif

/*! @addtogroup groupcommon The Common Part
 * @brief The general part.
 * @details This part refers to the general methods and definitions.
 *  @{
 */

//! @cond
#define atom_setfloat(a, f) SETFLOAT(a, f)
#define atom_setlong(a, l) SETFLOAT(a, (t_float)l)
#define atom_setsym(a, s) SETSYMBOL(a, s)
#define atom_getlong(a) (long)atom_getfloat(a)
#define atom_gettype(a) (a)[0].a_type
//! @endcond

//! The error type
typedef long t_pd_err;
//! The standart method
typedef void (*t_typ_method)(void* x, ...);
//! The return method
typedef void* (*t_ret_method)(void* x, ...);
//! The error method
typedef t_pd_err (*t_err_method)(void* x, ...);

//! The pre-defined ("null") t_symbol*
extern t_symbol* s_null;
//! The pre-defined obj t_symbol*
extern t_symbol* s_obj;
//! The pre-defined atom t_symbol*
extern t_symbol* s_atom;
//! The pre-defined attr_modified t_symbol*
extern t_symbol* s_attr_modified;
//! The pre-defined size t_symbol*
extern t_symbol* s_size;
//! The pre-defined int t_symbol*
extern t_symbol* s_int;
//! The pre-defined long t_symbol*
extern t_symbol* s_long;
//! The pre-defined double t_symbol*
extern t_symbol* s_double;
//! The pre-defined s_pinned t_symbol*
extern t_symbol* s_pinned;
//! The pre-defined s_iscicm t_symbol*
extern t_symbol* s_iscicm;
//! The pre-defined attr_size t_symbol*
extern t_symbol* s_attr_size;
//! The pre-defined s_color_black_hex t_symbol*
extern t_symbol* s_color_black_hex;

extern t_symbol* s_prop_label;
extern t_symbol* s_prop_label_align;
extern t_symbol* s_prop_label_valign;
extern t_symbol* s_prop_label_position;
extern t_symbol* s_prop_label_side;
extern t_symbol* s_value_label_align_left;
extern t_symbol* s_value_label_align_center;
extern t_symbol* s_value_label_align_right;
extern t_symbol* s_value_label_valign_top;
extern t_symbol* s_value_label_valign_center;
extern t_symbol* s_value_label_valign_bottom;
extern t_symbol* s_value_label_side_left;
extern t_symbol* s_value_label_side_top;
extern t_symbol* s_value_label_side_right;
extern t_symbol* s_value_label_side_bottom;

//! @cond
typedef struct _namelist /* element in a linked list of stored strings */
{
    struct _namelist* nl_next; /* next in list */
    char* nl_string; /* the string */
} t_namelist;

EXTERN t_namelist* sys_searchpath;

//! @endcond

/** @} */

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

//! Macros that define the a pi
#define EPD_PI (3.141592653589793238462643383279502884f)
//! Macros that define the a 2pi
#define EPD_2PI (6.283185307179586476925286766559005f)
//! Macros that define the a pi over 2
#define EPD_PI2 (1.57079632679489661923132169163975144f)
//! Macros that define the a pi over 4
#define EPD_PI4 (0.785398163397448309615660845819875721f)

/**
 * @enum elayer_flags
 * @brief The flags that defines the status of a layer.
 * @details It define all possible the status of a layer.
 */
typedef enum elayer_flags {
    EGRAPHICS_OPEN = 0, /*!< Open. */
    EGRAPHICS_CLOSE = -1, /*!< Closed. */
    EGRAPHICS_INVALID = -2, /*!< Invalid. */
    EGRAPHICS_TODRAW = -3 /*!< To be drawn. */
} elayer_flags;

/**
 * @enum etextanchor_flags
 * @brief The flags that defines the text anchor.
 * @details It define all text anchor.
 */
typedef enum etextanchor_flags {
    ETEXT_UP = 0, /*!< Up and center. */
    ETEXT_UP_LEFT = 1, /*!< Up and left. */
    ETEXT_UP_RIGHT = 2, /*!< Up and right. */
    ETEXT_DOWN = 3, /*!< Down and center. */
    ETEXT_DOWN_LEFT = 4, /*!< Down and left. */
    ETEXT_DOWN_RIGHT = 5, /*!< Down and right. */
    ETEXT_LEFT = 6, /*!< Left. */
    ETEXT_RIGHT = 7, /*!< Right. */
    ETEXT_CENTER = 8 /*!< Center. */

} etextanchor_flags;

/**
 * @enum etextwrap_flags
 * @brief The flags that defines if the text should be wrapped.
 * @details It define true or false.
 */
typedef enum etextwrap_flags {
    ETEXT_NOWRAP = 0, /*!< False. */
    ETEXT_WRAP = 1 /*!< True. */
} etextwrap_flags;

/**
 * @enum etextjustify_flags
 * @brief The flags that define the text justification.
 * @details It define all text justification.
 */
typedef enum etextjustify_flags {
    ETEXT_JLEFT = 0, /*!< Left. */
    ETEXT_JRIGHT = 1, /*!< Right. */
    ETEXT_JCENTER = 2 /*!< Center. */
} etextjustify_flags;

/**
 * @enum egraphics_types
 * @brief The types of graphical object.
 * @details It define all the graphical type.
 */
typedef enum {
    E_GOBJ_INVALID = 0, /*!< This type is invalid. */
    E_GOBJ_PATH = 1, /*!< This is a path. */
    E_GOBJ_TEXT = 2, /*!< This is a text. */
    E_GOBJ_SHAPE = 3 /*!< This is a shape. */
} egraphics_types;

/**
 * @enum epath_types
 * @brief The types of path.
 * @details It define all the path type.
 */
typedef enum {
    E_PATH_MOVE = 0, /*!< This type is move. */
    E_PATH_LINE = 1, /*!< This type is line. */
    E_PATH_CURVE = 2, /*!< This type is curve. */
    E_PATH_CLOSE = 3, /*!< This type is close. */
    E_PATH_CIRCLE = 4
} epath_types;

/**
 * @enum eshape_types
 * @brief The types of shape.
 * @details It define soem of the shape type.
 */
typedef enum {
    E_SHAPE_OVAL = 0, /*!< This shape is oval. */
    E_SHAPE_ARC = 1, /*!< This shape is arc. */
    E_SHAPE_IMAGE = 2, /*!< This shape is image. */
    E_SHAPE_RECT = 3 /*!< This shape is rectangle. */
} eshape_types;

enum eclip_flags {
    E_CLIP_NONE = 0x0,
    E_CLIP_MIN = 0x1,
    E_CLIP_MAX = 0x2,
    E_CLIP_MINMAX = E_CLIP_MIN & E_CLIP_MAX
};

/**
 * @struct t_pt
 * @brief A point structure.
 * @details It contains the members x and y for abscissa and ordinate.
 */
struct t_pt {
    float x; /*!< The abscissa coordiante. */
    float y; /*!< The ordiante coordiante. */

    constexpr t_pt()
        : x(0)
        , y(0)
    {
    }

    constexpr t_pt(epath_types path, float y_)
        : x(path)
        , y(y_)
    {
    }

    constexpr t_pt(eshape_types shape, float y_)
        : x(shape)
        , y(y_)
    {
    }

    constexpr t_pt(float x_, float y_)
        : x(x_)
        , y(y_)
    {
    }

    constexpr t_pt(double x_, double y_)
        : x(x_)
        , y(y_)
    {
    }

    bool operator==(const t_pt& pt) const { return x == pt.x && y == pt.y; }
    bool operator!=(const t_pt& pt) const { return !this->operator==(pt); }
};

/**
 * @struct t_rect
 * @brief A rectangle structure.
 * @details It contains the members x, y, width and height for abscissa and ordinate and size.
 */
struct t_rect {
    float x; /*!< The abscissa coordiante. */
    float y; /*!< The ordiante coordiante. */
    float width; /*!< The width of the rectangle */
    float height; /*!< The height of the rectangle */

    bool operator==(const t_rect& r) const { return r.x == x && r.y == y && r.width == width && r.height == height; }
    bool operator!=(const t_rect& r) const { return !this->operator==(r); }
};

/**
 * @struct t_matrix
 * @brief A matrix structure.
 * @details It contains the members xx, yx, xy, yy, x0 and y0
 */
struct t_matrix {
    float xx; /*!< The abscissa-abscissa translation of the matrix */
    float yx; /*!< The abscissa-ordiante translation of the matrix */
    float xy; /*!< The ordiante-abscissa translation of the matrix */
    float yy; /*!< The ordiante-ordiante translation of the matrix */
    float x0; /*!< The abscissa origin of the matrix */
    float y0; /*!< The ordiante origin of the matrix */
};

/**
 * @struct t_rgb
 * @brief A rgb color structure.
 * @details It contains the members red, green and blue.
 */
typedef struct t_rgb {
    float red; /*!< The red value. */
    float green; /*!< The green value. */
    float blue; /*!< The blue value. */
} t_rgb;

/**
 * @struct t_rgba
 * @brief A rgba color structure.
 * @details It contains the members red, green, blue and alpha.
 */
typedef struct t_rgba {
    float red; /*!< The red value. */
    float green; /*!< The green value. */
    float blue; /*!< The blue value. */
    float alpha; /*!< The alpha value. */
} t_rgba;

/**
 * @struct t_hsl
 * @brief A hsl color structure.
 * @details It contains the members hue, saturation and lightness.
 */
typedef struct t_hsl {
    float hue; /*!< The hue value. */
    float saturation; /*!< The saturation value. */
    float lightness; /*!< The lightness value. */
} t_hsl;

/**
 * @struct t_hsla
 * @brief A hsla color structure.
 * @details It contains the members hue, saturation, lightness and alpha.
 */
typedef struct t_hsla {
    float hue; /*!< The hue value. */
    float saturation; /*!< The saturation value. */
    float lightness; /*!< The lightness value. */
    float alpha; /*!< The alpha value. */
} t_hsla;

/**
 * @struct t_capstyle
 * @brief A capstyle enum
 */
typedef enum t_capstyle {
    ECAPSTYLE_BUTT = 0, /*!< Default. A flat edge is added to each end of the line */
    ECAPSTYLE_ROUND = 1, /*!< A rounded end cap is added to each end of the line */
    ECAPSTYLE_SQUARE = 2 /*!< A square end cap is added to each end of the line */
} t_capstyle;

/**
 * @struct t_dashstyle
 * @brief A dashstyle enum
 */
typedef enum t_dashstyle {
    EDASHSTYLE_NONE = 0, /*!< Default. No dash line pattern */
    EDASHSTYLE_24 = 1, /*!<  */
    EDASHSTYLE_64 = 2 /*!<  */
} t_dashstyle;

/**
 * @struct t_dashstyle
 * @brief A dashstyle enum
 */
typedef enum t_smooth {
    ESMOOTH_NONE = 0, /*!< Default. No dash line pattern */
    ESMOOTH_RAW = 1, /*!<  */
    ESMOOTH_BEZIER = 2 /*!<  */
} t_smooth;

//! The pre-defined black t_rgba
extern const t_rgba rgba_black;
//! The pre-defined grey dark t_rgba
extern const t_rgba rgba_greydark;
//! The pre-defined grey t_rgba
extern const t_rgba rgba_grey;
//! The pre-defined grey light t_rgba
extern const t_rgba rgba_greylight;
//! The pre-defined white t_rgba
extern const t_rgba rgba_white;
//! The pre-defined blue t_rgba
extern const t_rgba rgba_blue;
//! The pre-defined green t_rgba
extern const t_rgba rgba_green;
//! The pre-defined red t_rgba
extern const t_rgba rgba_red;

/**
 * @struct t_efont
 * @brief The font structure.
 * @details It contains the informations of a font.
 */
typedef struct t_efont {
    t_symbol* c_family; /*!< The family of the font (times, helvetica, ect.). */
    t_symbol* c_slant; /*!< The slant of the font (regular, italic, etc.). */
    t_symbol* c_weight; /*!< The weight of the font (normal, bold). */
    float c_size; /*!< The size of the font. */
    int c_sizereal; /*!< The platform's size of the font. */
} t_efont;

/**
 * @struct t_etext
 * @brief The text structure.
 * @details It contains the all the informations to be drawn.
 */
typedef struct t_etext {
    t_rgba c_color; /*!< The color of the text. */
    t_efont c_font; /*!< The font of the text. */
    t_rect c_rect; /*!< The rectangle of the text. */
    const char* c_text; /*!< The text. */
    char c_buf[64];
    char c_is_buffer_used;
    etextanchor_flags c_anchor; /*!< The anchor of the text. */
    etextjustify_flags c_justify; /*!< The justification of the text. */
} t_etext;

/**
 * @struct t_eimage
 * @brief The image structure.
 * @details It contains the all the informations to be drawn.
 */
typedef struct t_eimage {
    const char* data_base64;
    t_symbol* name;
    uint16_t width;
    uint16_t height;
    etextanchor_flags anchor;
} t_eimage;

/**
 * @struct t_egobj
 * @brief The  drawing object.
 * @details It contains the all the informations to be drawn.
 * @todo should keep the allocated memory
 */
typedef struct t_egobj {
    std::vector<t_pt> e_points; /*!< The points of the graphical object. */
    t_efont e_font; /*!< The font of the graphical object. */
    const char* e_text; /*!< The text of the graphical object. */
    t_eimage* e_image; /*!< The image of the graphical object. */

    float e_width; /*!< The line width of the graphical object. */
    uint32_t e_color; /*!< The color of the graphical object. */

    t_capstyle e_capstyle; /*!< The line capstyle of the graphical object. */
    t_dashstyle e_dashstyle; /*!< The line dashstyle of the graphical object. */
    t_smooth e_smooth; /*!< The line smooth of the graphical object. */

    etextanchor_flags e_anchor; /*!< The anchor of the graphical object. */
    etextjustify_flags e_justify; /*!< The justification of the graphical object. */
    egraphics_types e_type; /*!< The type of the graphical object. */
    bool e_filled; /*!< The filled state of the graphical object. */
} t_egobj;

/**
 * @struct t_elayer
 * @brief The  drawing layer.
 * @details It contains the all the informations and the graphical objects to be drawn.
 */
typedef struct t_elayer {
    t_object* e_owner; /*!< The layer owner. */
    t_symbol* e_name; /*!< The layer name. */
    t_symbol* e_id; /*!< The layer canvas ID. */
    std::vector<t_egobj> e_objects; /*!< The layer objects. */
    t_rect e_rect; /*!< The layer size. */
    t_matrix e_matrix; /*!< The layer matrix. */
    t_egobj e_new_objects; /*!< The layer new object. */

    uint32_t e_color; /*!< The layer color. */
    uint16_t e_line_width; /*!< The layer line width. */

    elayer_flags e_state; /*!< The layer state. */
    t_capstyle e_line_capstyle; /*!< The layer line capstyle. */
    t_dashstyle e_line_dashstyle; /*!< The layer line dashstyle. */
    t_smooth e_line_smooth; /*!< The layer line dashstyle. */
    bool e_optimize; /*!< optimize line drawing */
} t_elayer;

/** @} */

/*! @addtogroup grouppopup The Popup Window Part
 * @brief The t_epopup part.
 * @details This part refers to the methods and structures that can be used by all the t_epopup structures.
 *  @{
 */

/**
 * @struct t_epopup
 * @brief The popup structure.
 * @details It contains the informations to show and retieve a popup.
 */
typedef struct t_epopup {
    t_symbol* c_name; /*!< The name of the popup. */
    t_symbol* c_send; /*!< The name of the owner. */
} t_epopup;

/** @} */

/*! @addtogroup groupclass The Class Part
 * @brief The t_eclass part.
 * @details This part refers to the methods and structures that can be used by all the t_eclass structures.
 *  @{
 */

//! Macros that define the a default object
#define CLASS_OBJ gensym("obj")
//! Macros that define the a GUI box
#define CLASS_BOX gensym("box")

typedef void (*t_mouseenter_method)(void* x);
typedef void (*t_mouseleave_method)(void* x);
typedef void (*t_mousemove_method)(void* x, t_glist*, t_pt, long);
typedef void (*t_mousedown_method)(void* x, t_glist*, t_pt, t_pt, long);
typedef void (*t_mouseup_method)(void* x, t_glist*, t_pt, long);
typedef void (*t_mousewheel_method)(void* x, t_pt, long, float);
typedef void (*t_dblclick_method)(void* x, t_glist*, t_pt, long);
typedef void (*t_rightclick_method)(void* x, t_pt, t_pt);
typedef void (*t_popup_method)(void* x, t_symbol*, long, t_pt);

/**
 * @struct t_ewidget
 * @brief The default method of a class.
 * @details It contains the adress of the default methods of a class.
 */
typedef struct t_ewidget {
    t_getrectfn w_getrectfn; /*!< The native Pd get size method. */
    t_displacefn w_displacefn; /*!< The native Pd displace method. */
    t_selectfn w_selectfn; /*!< The native Pd selection method. */
    t_activatefn w_activatefn; /*!< The native (dummy) Pd active method. */
    t_deletefn w_deletefn; /*!< The native Pd deletion method. */
    t_visfn w_visfn; /*!< The native Pd visible method. */
    t_clickfn w_clickfn; /*!< The native Pd click method. */
    t_typ_method w_assist; /*!< The dummy iolets assist method. */
    t_typ_method w_paint; /*!< The paint method. */
    t_typ_method w_create; /*!< The widget after create method. */
    t_typ_method w_erase; /*!< The widget before erase method. */
    t_mouseenter_method w_mouseenter; /*!< The mouse enter method. */
    t_mouseleave_method w_mouseleave; /*!< The mouse leave method. */
    t_mousemove_method w_mousemove; /*!< The mouse move method. */
    t_mousedown_method w_mousedown; /*!< The mouse down method. */
    t_typ_method w_mousedrag; /*!< The mouse drag method. */
    t_mouseup_method w_mouseup; /*!< The mouse up method. */
    t_mousewheel_method w_mousewheel; /*!< The mouse wheel method. */
    t_dblclick_method w_dblclick; /*!< The mouse double click method. */
    t_rightclick_method w_rightclick; /*!< The mouse double click method. */
    t_typ_method w_key; /*!< The key method. */
    t_typ_method w_keyfilter; /*!< The key filter method. */
    t_typ_method w_getdrawparameters; /*!< The get draw parameter method. */
    t_typ_method w_save; /*!< The save method. */
    t_typ_method w_dosave; /*!< The real save method. */
    t_popup_method w_popup; /*!< The popup method. */
    t_typ_method w_dsp; /*!< The dsp method. */
    t_typ_method w_oksize; /*!< The size validation method. */
    t_err_method w_notify; /*!< The notification method. */
    t_typ_method w_write; /*!< The write to file method. */
    t_typ_method w_read; /*!< The read from file method. */
    t_typ_method w_onzoom; /*!< Zoom callback. */
} t_ewidget;

/**
 * @struct t_eattr
 * @brief The attribute.
 * @details It contains the members and the methods for an attribute. It is not already an object but perhaps it will be in the futur.
 */
typedef struct t_eattr {
    t_object obj; /*!< The dummy object. */
    t_symbol* name; /*!< The name of the attribute. */
    t_symbol* type; /*!< The type of the attribute (int, long, float,d double, rgba, etc.). */
    t_symbol* category; /*!< The dummy category of the attribute. */
    t_symbol* label; /*!< The label of the attribute. */
    t_symbol* style; /*!< The style of the attribute (checkbutton, color, number, entry, menu). */
    t_symbol* units; /*!< The units of the attribute. */
    t_symbol* defvals; /*!< The default value of the attribute. */
    t_symbol** itemslist; /*!< The available items of an attribute if it is a menu. */
    size_t itemssize; /*!< The number of available items of an attribute if it is a menu. */
    t_err_method getter; /*!< The getter method of the attribute. */
    t_err_method setter; /*!< The setter method of the attribute. */
    size_t offset; /*!< The offset of the attribute in the object structure. */
    size_t sizemax; /*!< The maximum size of the attribute if the attribute is an array. */
    size_t size; /*!< The size of the attribute if the attribute is an array. */
    t_float minimum; /*!< The minimum value of the attribute. */
    t_float maximum; /*!< The maximum value of the attribute. */
    t_float step; /*!< The increment or decrement step calue of the attribute. */
    int order; /*!< The dummy order of the attribute. */
    eclip_flags clipped; /*!< If the attribute is clipped if it's value or an array of numerical values. */
    bool save; /*!< If the attribute should be saved. */
    bool paint; /*!< If the attribute should repaint the t_ebox when it has changed. */
    bool invisible; /*!< If the attribute is invisible. */
} t_eattr;

enum t_eattr_op {
    EATTR_OP_ASSIGN,
    EATTR_OP_ADD,
    EATTR_OP_SUB,
    EATTR_OP_MUL,
    EATTR_OP_DIV,
    EATTR_OP_UNKNOWN
};

/**
 * @struct t_eclass
 * @brief The class.
 * @details It contains the Pure Data default class with extra methods and attributes.
 */
typedef struct t_eclass {
    t_class c_class; /*!< The default class. */
    // do not remove!
    // in Pd 0.48 t_class* next added into t_class, and sizeof(t_class) grown to 8 bytes on x86_64
    // this padding added to prevent field rewriting values
    char c_padding[8];
    bool c_box; /*!< The marker if the class is GUI. */
    bool c_dsp; /*!< The marker if the class is DSP. */
    t_ewidget c_widget; /*!< The extra widget methods. */
    t_eattr** c_attr; /*!< The attributes. */
    size_t c_nattr; /*!< The number of attributes. */
} t_eclass;

/** @} */

/*! @addtogroup groupobj The BASIC Object Part
 * @brief The t_eobj part.
 * @details This part refers to the methods and structures that can be used by all the t_eobj structures.
 *  @{
 */

/**
 * @struct t_eproxy
 * @brief The Proxy object.
 * @details It contains the proxy class, the eobj owner and the index of the proxy.
 */
typedef struct t_eproxy {
    t_pd p_pd; /*!< The class object. */
    t_object* p_owner; /*!< The pointer to the eobj owner. */
    t_inlet* p_inlet; /*!< The pointer to the inlet. */
    int p_index; /*!< The index of the proxy. */
} t_eproxy;

/**
 * @struct t_eobj
 * @brief The default object.
 * @details It contains the Pd object, the canvas pointer and members for proxy inlets.
 * This should be used for no graphical object that don't have signal processing methods.
 */
typedef struct t_eobj {
    t_object o_obj; /*!< The Pd object. */
    t_symbol* o_id; /*!< The object ID. */
    t_canvas* o_canvas; /*!< The canvas that own the object. */
    t_eproxy** o_proxy; /*!< The array of proxy inlets. */
    int o_nproxy; /*!< The number of proxy inlets. */
    int o_current_proxy; /*!< The index of the current proxy inlet used */
} t_eobj;

/** @} */

/*! @addtogroup groupdspobj The DSP Object Part
 * @brief The t_edspobj and t_edspbox part.
 * @details This part refers to the methods and structures that can be used by all the t_edspobj structures.
 *  @{
 */

/**
 * @struct t_edsp
 * @brief The DSP structure.
 * @details It contains all the members for signal processing.
 * This should be used for no graphical object that have signal processing methods.
 */
typedef struct t_edsp {
    float d_float; /*!< The float member to initialize the signal method. */
    long d_size; /*!< The number of signal inlets and outlets. */
    t_int* d_vectors; /*!< The vector that contains all the pointers for the perform method. */
    long d_flags; /*!< The flags to initialize the perform method. */
    void* d_user_param; /*!< The user parameters to pass through the perform method. */
    t_float** d_sigs_out; /*!< The array of signal vectors. */
    t_float* d_sigs_real; /*!< The real array of signal. */
    t_typ_method d_perform_method; /*!< The user perform method. */
    long d_misc; /*!< The flag that could be inplace or not. */
} t_edsp;

/**
 * @enum edsp_flags
 * @brief The flags that discribe the behavior of a t_edspobj.
 * @details It define all the behavior of a t_edspobj.
 */
typedef enum {
    E_INPLACE = 0, /*!< The signal processing can be inplace. */
    E_NO_INPLACE = 1 /*!< The signal processing can't be inplace. */
} edsp_flags;

/**
 * @struct t_edspobj
 * @brief The DSP object.
 * @details It contains the t_eobj with the t_edsp structure.
 * This should be used for no graphical object that have signal processing methods.
 */
typedef struct t_edspobj {
    t_eobj d_obj; /*!< The default object. */
    t_edsp d_dsp; /*!< The dsp structure. */
} t_edspobj;

/** @} */

/*! @addtogroup groupbox The GUI Object Part
 * @brief The t_ebox and t_edspbox part.
 * @details This part refers to the methods and structures that can be used by all the t_ebox structures.
 *  @{
 */

/**
 * @enum emod_flags
 * @brief The mouse modifiers.
 * @details It define mouse modifiers.
 */
typedef enum emod_flags {
    EMOD_NONE = 0, /*!< Nothing. */
    EMOD_SHIFT = 1, /*!< Shift. */
    EMOD_MAJ = 2, /*!< Maj. */
    EMOD_CTRL = 4, /*!< Control. */
    EMOD_CMD = 8, /*!< Command. */
    EMOD_ALT = 16, /*!< Alt. */
    EMOD_RIGHT = 32 /*!< Right click. */
} emod_flags;

/**
 * @enum ekey_flags
 * @brief The system keys.
 * @details It define the system keys.
 */
typedef enum ekey_flags {
    EKEY_DEL = 0, /*!< The delete key. */
    EKEY_TAB = 1, /*!< The tabulation key. */
    EKEY_ENTER = 2, /*!< The return key. */
    EKEY_ESC = 3 /*!< The escape key. */

} ekey_flags;

//! @cond
typedef enum eselitem_flags {
    EITEM_NONE = 0,
    EITEM_OBJ = 1,
    EITEM_INLET = 2,
    EITEM_OUTLET = 3,
    EITEM_BOTTOM = 4,
    EITEM_CORNER = 5,
    EITEM_RIGHT = 6
} eselitem_flags;
//! @endcond

/**
 * @enum ebox_flags
 * @brief The flags that discribe the behavior of a t_ebox.
 * @details It define all the behavior of a t_ebox.
 */
typedef enum {
    EBOX_GROWNO = (1 << 4), /*!< The width and the height can't be modified. */
    EBOX_GROWLINK = (1 << 5), /*!< The width and the height are linked. */
    EBOX_GROWINDI = (1 << 6), /*!< The width and the height are independant. */
    EBOX_IGNORELOCKCLICK = (1 << 7) /*!< The t_ebox ignore the mouse events. */
} ebox_flags;

/**
 * @enum ebox_cursors
 * @brief The cursors that can be used.
 * @details It define all the available cursors.
 */
typedef enum t_cursor {
    ECURSOR_LEFT_PTR = 0, /*!< The left_ptr string. */
    ECURSOR_CENTER_PTR = 1, /*!< The center_ptr string. */
    ECURSOR_PLUS = 2, /*!< The plus string. */
    ECURSOR_HAND = 3, /*!< The hand2 string. */
    ECURSOR_CIRCLE = 4, /*!< The circle string. */
    ECURSOR_X = 5, /*!< The X_cursor string. */
    ECURSOR_BOTTOM = 6, /*!< The bottom_side string. */
    ECURSOR_RIGHT_CORNER = 7, /*!< The bottom_right_corner string. */
    ECURSOR_RIGHT_SIDE = 8, /*!< The right_side string. */
    ECURSOR_LEFT_SIDE = 9, /*!< The left_side string. */
    ECURSOR_EXCHANGE = 10, /*!< The exchange string. */
    ECURSOR_XTERM = 11, /*!< The xterm string. */
    ECURSOR_MOVE = 12, /*!< The move cursor */
    ECURSOR_VDOUBLE_ARROW = 13, /*!< The sb_v_double_arrow string. */
    ECURSOR_HDOUBLE_ARROW = 14 /*!< The sb_h_double_arrow string. */
} ebox_cursors;

/**
 * @struct t_edrawparams
 * @brief The  drawing parameters.
 * @details It contains the default parameters of a ebox.
 */
typedef struct t_edrawparams {
    t_rgba d_bordercolor; /*!< The border color. */
    t_rgba d_boxfillcolor; /*!< The background color. */
    t_rgba d_labelcolor; /*!< The label color. */
    bool d_hideiolets; /*!< hide standard iolets, for ui.icon for example */
    bool d_hideborder; /*!< hide standard border, for ui.link for example */
} t_edrawparams;

/**
 * @struct t_ebox
 * @brief The GUI object.
 * @details It contains the t_eobj with all the members for graphical behavior.
 * This should be used for graphical object that don't have signal processing methods.
 */
typedef struct t_ebox_ {
    t_eobj b_obj; ///<The  object.

    t_symbol* b_receive_id; /*!< The object user ID. */
    t_symbol* b_send_id; /*!< The object send ID. */
    t_symbol* b_objpreset_id; /*!< The object preset ID. */

    t_symbol* b_canvas_id; /*!< The canvas ID. */
    t_symbol* b_drawing_id; /*!< The drawing ID. */
    t_symbol* b_all_id; /*!< The global ID. */

    long b_flags; /*!< The ebox flags. */
    t_rect b_rect; /*!< The ebox rectangle. */
    t_rect b_rect_last; /*!< The ebox previous rectangle. */
    t_efont b_font; /*!< The ebox font. */
    int b_pinned; /*!< If the ebox is pinned to the canvas. */
    int b_selected_box; /*!< The selected state */
    int b_selected_item; /*!< The items selected. */
    int b_selected_inlet; /*!< The inlet selected. */
    int b_selected_outlet; /*!< The outlet selected. */
    float b_zoom;

    bool b_mouse_down; /*!< The mouse state. */
    bool b_resize; /*!< Widget is in resize state */
    bool b_resize_redraw_all; /*!< Widget in resize state and redraw all layers */

    bool b_visible; /*!< The visible state. */
    bool b_ready_to_draw; /*!< The ebox state for drawing. */
    bool b_have_window; /*!< The ebox window state. */
    bool b_isinsubcanvas; /*!< If the box is in a sub canvas. */
    t_edrawparams b_boxparameters; /*!< The ebox parameters. */

    std::vector<t_elayer>* b_layers; /*!< The ebox layers. */

    t_symbol* b_label; /*!< The UI label. */
    t_symbol* label_align; /*!< The UI label align: left center or right */
    t_symbol* label_valign; /*!< The UI label vertical align: top, center or bottom */
    t_symbol* label_side; /*!< The UI label anchor side: top, left, right, or bottom */
    int label_inner;
    int label_margins[2];
    t_cursor cursor;

    t_canvas* wis_canvas;
} t_ebox;

/** @} */

/**
 * @struct t_edspbox
 * @brief The GUI DSP object.
 * @details It contains the t_eobj with all the members for graphical behavior and signal processing.
 * This should be used for graphical object that have signal processing methods.
 * @ingroup groupbox groupdspobj
 */
typedef struct t_edspbox {
    t_ebox e_box; /*!< The  DSP object. */
    t_edsp d_dsp; /*!< The dsp structure. */
} t_edspbox;

#endif
