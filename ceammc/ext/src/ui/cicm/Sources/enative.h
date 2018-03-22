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
 * @file enative.h
 * @brief The header that declares all the natives methods of Pd. You should never include the file, this is used for documentation.
 * @author Pierre Guillot
 */

#ifndef DEF_EPD_NATIVE
#define DEF_EPD_NATIVE

/*! @addtogroup groupnative The Native Part
 * @brief The native part.
 * @details This part refers to the Pd natives methods and definitions. You should never include the part, this is used for documentation.
 *  @{
 */

//! The standart method aka method
typedef void        (*t_method)(void* x, ...);

//! The default class flags
#define CLASS_DEFAULT 0
//! The internal pd class flags
#define CLASS_PD 1
//! The internal array or scalar class flags (don't use it gor GUI)
#define CLASS_GOBJ 2
//! The internal text class flags (you should prefer to use CLASS_DEFAULT)
#define CLASS_PATCHABLE 3
//! The no default first inlet class flags should use it when an t_object should'nt have any inlet
#define CLASS_NOINLET 8

/**
 * @struct t_symbol
 * @brief The t_symbol struture.
 * @details The t_symbol is a unique symbol that can be used to bind objects together or as a unique id for strings that can be faster to compare. Use gensym to generate a t_symbol.
 */
typedef struct t_symbol
{
    char *s_name;                /*!< The name of the t_symbol. */
    struct t_class **s_thing;    /*!< The binding list of the t_symbol. */
    struct t_symbol *s_next;     /*!< The next t_symbol. */
} t_symbol;

/**
 * @struct t_binbuf
 * @brief The t_binbuf struture.
 * @details The t_binbuf is just an holder for an array of t_atoms.
 */
typedef struct t_binbuf
{
    int b_n;        /*!< The number of t_atom. */
    t_atom *b_vec;  /*!< The array of t_atom. */
} t_binbuf;

/**
 * @union t_word
 * @brief The t_word struture.
 * @details The internal value of a t_atom .
 */
typedef union word
{
    t_float w_float;            /*!< The float value. */
    t_symbol *w_symbol;         /*!< The t_symbol. */
    t_gpointer *w_gpointer;
    t_array *w_array;           /*!< The array. */
    struct _binbuf *w_binbuf;   /*!< The binbuf. */
    int w_index;
} t_word;

/**
 * @enum t_atomtype
 * @brief The flags that discribes the type of an t_atom.
 * @details It define all possible type of an t_atom.
 */
typedef enum
{
    A_NULL,     /*!< Nothing. */
    A_FLOAT,    /*!< A float. */
    A_SYMBOL,   /*!< A t_symbol. */
    A_POINTER,
    A_SEMI,
    A_COMMA,
    A_DEFFLOAT,
    A_DEFSYM,
    A_DOLLAR,
    A_DOLLSYM,
    A_GIMME,    /*!< An array of t_atoms or a function with the parameters int and t_atom*. */
    A_CANT      /*!< Nothing or a function that can be called only internally with any kind of parameters. */
}  t_atomtype;

/**
 * @struct t_atom
 * @brief The t_atom struture.
 * @details The t_atom is the formatted structure used to communicates between objects in Pd.
 */
typedef struct t_atom
{
    t_atomtype a_type;  /*!< The type of the t_atom. */
    union word a_w;     /*!< The value of the t_atom. */
} t_atom;

/**
 * @struct t_outlet
 * @brief The t_outlet struture.
 * @details The t_outlet is owns the necessary member to send messages to other object.
 */
typedef struct t_outlet
{
    t_object *o_owner;              /*!< The onwer of the t_outlet. */
    struct _outlet *o_next;         /*!< The next t_outlet in the t_object or NULL if it is the last one. */
    t_outconnect *o_connections;    /*!< The connection of the t_outlet. */
    t_symbol *o_sym;                /*!< The symbol that defines caht type of message the outlet send. */
} t_outlet;

//! pure datum: nothing but a class pointer
typedef t_class *t_pd;

/**
 * @struct t_gobj
 * @brief The graphical t_object struture.
 * @details The t_gobj is owns the t_class of an t_object and the pointer to the next t_object.
 */
typedef struct t_gobj
{
    t_pd g_pd;              /*!< pure datum header (class) */
    struct t_gobj *g_next;  /*!< next in list */
} t_gobj;

/**
 * @struct t_object
 * @brief The t_object struture.
 * @details The t_object is owns the necessary member of a patchable object.
 */
typedef struct t_object
{
    t_gobj te_g;                /*!< header for graphical t_object */
    t_binbuf *te_binbuf;        /*!< holder for the text */
    t_outlet *te_outlet;        /*!< linked list of outlets */
    t_inlet *te_inlet;          /*!< linked list of inlets */
    short te_xpix;              /*!< x location (within the toplevel) */
    short te_ypix;              /*!< y location (within the toplevel) */
    short te_width;             /*!< requested width in chars, 0 if auto */
    unsigned int te_type:2;     /*!< from defs below */
} t_object;

/**
 * @struct t_clock
 * @brief The t_clock struture.
 * @details The t_clock is owns the necessary member of a clock. A t_clock is used to defer the excecution of a method.
 */
typedef struct t_clock
{
    double c_settime;       /*!< in TIMEUNITS; <0 if unset */
    void *c_owner;          /*!< The owner */
    t_clockmethod c_fn;     /*!< The method to call */
    struct t_clock *c_next; /*!< The next to t_clock */
    t_float c_unit;         /*!< >0 if in TIMEUNITS; <0 if in samples */
} t_clock;

//! The pre-defined pointer t_symbol
EXTERN t_symbol s_pointer;
//! The pre-defined float t_symbol
EXTERN t_symbol s_float;
//! The pre-defined symbol t_symbol
EXTERN t_symbol s_symbol;
//! The pre-defined bang t_symbol
EXTERN t_symbol s_bang;
//! The pre-defined list t_symbol
EXTERN t_symbol s_list;
//! The pre-defined anything t_symbol
EXTERN t_symbol s_anything;
//! The pre-defined signal t_symbol
EXTERN t_symbol s_signal;

/*!
 * \fn          t_symbol *gensym(const char *s)
 * \brief       Retrieves the t_symbol that match with the string.
 * \details     The function allocates a new t_symbol if it doesn't already exist, otherwise it return the t_symbol that matchs to the string.
 * \param s     The name of the t_symbol.
 * \return The pointer to the t_symbol.
 */
t_symbol *gensym(const char *s);

/*!
 * \fn          t_clock *clock_new(void *owner, method fn)
 * \brief       Allocates a new t_clock.
 * \details     The function allocates a new t_clock and attaches it to a function.
 * \param owner The owner of the t_clock.
 * \param fn    The fn to call.
 * \return The pointer to the t_clock.
 */
t_clock *clock_new(void *owner, method fn);

/*!
 * \fn          void clock_free(t_clock *x)
 * \brief       Frees a t_clock.
 * \param x     The t_clock.
 */
void clock_free(t_clock *x);

/*!
 * \fn          void clock_delay(t_clock *x, double delaytime)
 * \brief        defer the excecution of the t_clock's method.
 * \param x     The t_clock.
 * \param delaytime The time to wait.
 */
void clock_delay(t_clock *x, double delaytime);

/*!
 * \fn          t_outlet *outlet_new(t_object *owner, t_symbol *s)
 * \brief       Allocates and initializes a new t_outlet for an object.
 * \details     The function allocates a initializes a new t_outlet for an object. The symbol is generally one of the pd default message s_bang, s_symbol, s_list, s_anything, ect.
 * \param owner The owner of the outlet.
 * \param s     The symbol that of the outlet.
 * \return The pointer to the t_outlet.
 */
t_outlet *outlet_new(t_object *owner, t_symbol *s);

/*!
 * \fn          void outlet_bang(t_outlet *x)
 * \brief       Send a bang through an outlet
 * \param x     The outlet pointer.
 */
void outlet_bang(t_outlet *x);

/*!
 * \fn          void pd_bang(t_pd *x)
 * \brief       Send a bang to t_pd internal object
 * \param x     The t_pd pointer.
 */
void pd_bang(t_pd *x);


/** @} */
#endif
