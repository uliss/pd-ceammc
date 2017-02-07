/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 * \file eclass.h
 * \brief The header that declares all the methods for the t_ebox.
 * \author Pierre Guillot
 */

#ifndef DEF_ECLASS
#define DEF_ECLASS

#include "edefine.h"

/*! @addtogroup groupclass
 *  @{
 */

/*!
 * \fn          t_eclass* eclass_new(const char *name, t_typ_method newm, t_typ_method freem, size_t size, int flags, t_atomtype arg1, int arg2)
 * \brief       Allocates the memory and initialize a new t_eclass.
 * \details     Allocates and intializes the default Pd t_class then enlarges it to fit a t_eclass. \n This function should be replace class_new().
 * \param name  The class name.
 * \param newm  The new method.
 * \param freem The free method.
 * \param size  The size of the object.
 * \param flags The class flags.
 * \param arg1  The type of parameters the new function.
 * \param arg2  The type of object.
 * \return      This function return the new eclass
 */
t_eclass* eclass_new(const char *name, t_typ_method newm, t_typ_method freem, size_t size, int flags, t_atomtype arg1, int arg2);

/*!
 * \fn          void eclass_guiinit(t_eclass* c, long flags)
 * \brief       Initialize the t_eclass for the GUI behavior.
 * \details     Sets the defaults values and initializes the methods of the t_eclass for the GUI behavior.
 * \param c     The t_eclass pointer.
 * \param flags The dummy flags.
 */
void eclass_guiinit(t_eclass* c, long flags);

/*!
 * \fn          void eclass_init(t_eclass* c, long flags)
 * \brief       Initialize the t_eclass for the GUI behavior (deprecated).
 * \details     Sets the defaults values and initializes the methods of the t_eclass for the GUI behavior.
 * \param c     The t_eclass pointer.
 * \param flags The dummy flags.
* \deprecated Please use eclass_guiinit.
 */
void eclass_init(t_eclass* c, long flags) _FUNCTION_DEPRECTAED_;

/*!
 * \fn          t_pd_err eclass_register(t_symbol *name, t_eclass *c)
 * \brief       Initialize the t_eclass in a namespace and preprare the t_eclass to be ready.
 * \details     Should always call this function after setting all the methods and attributes.
 * \param name  The namespace obj or gui (dummy).
 * \param c     The t_eclass pointer.
 */
t_pd_err eclass_register(t_symbol *name, t_eclass *c);

/*!
 * \fn          void eclass_dspinit(t_eclass* c)
 * \brief       Initialize the t_eclass for the DSP behavior.
 * \details     Sets the defaults values and initializes the methods of the t_eclass for the DSP behavior.
 * \param c     The t_eclass pointer.
 */
void eclass_dspinit(t_eclass* c);

/*!
 * \fn          void eclass_addmethod(t_eclass* c, method m, const char* name, t_atomtype type, long anything)
 * \brief       Adds a method to the t_eclass.
 * \details     Adds and wraps the methods of the class. For the moment the funtion takes only 1 type the second type is a dummy one and should always be 0.
 * \param c     The t_eclass pointer.
 * \param m     The method.
 * \param name  The name of the method.
 * \param type  The type of the method.
 * \param dummy The dummy type that should be 0.
 */
void eclass_addmethod(t_eclass* c, t_typ_method m, const char* name, t_atomtype type, long dummy);

/*!
 * \fn          void eclass_new_attr_typed(t_eclass* c, const char* attrname, const char* type, long size, long maxsize, long flags, long offset)
 * \brief           Creates a new attribute.
 * \details         Allocates the memory and intializes an new attribute for an eclass. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param type      The attribute type
 * \param size      The attribute size
 * \param maxsize   The attribute max size
 * \param flags     The attribute flags (dummy)
 * \param offset    The attribute bit offset in the object structure
 */
void eclass_new_attr_typed(t_eclass* c, const char* attrname, const char* type, long size, long maxsize, long flags, long offset);

//! @cond
/*!
 * \fn       void eclass_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv)
 * \brief           The setter method of the attributes.
 * \param x         The object pointer
 * \param s         The attribute name
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms that contains the attributes values
 */
void eclass_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv);

/*!
 * \fn       void eclass_attr_getter(t_object* x, t_symbol *s, int* argc, t_atom** argv)
 * \brief           The getter method of the attributes.
 * \param x         The object pointer
 * \param s         The attribute name
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms that contains the attributes values
 */
void eclass_attr_getter(t_object* x, t_symbol *s, int* argc, t_atom** argv);

/*!
 * \fn       void eclass_attr_ceammc_getter(t_object* x, t_symbol *s, int* argc, t_atom** argv)
 * \brief           The getter method of the attributes.
 * \param x         The object pointer
 * \param s         The attribute name
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms that contains the attributes values
 */
void eclass_attr_ceammc_getter(t_object* x, t_symbol *s, int argc, t_atom* argv);

//! @endcond

/*!
 * \fn          void eclass_attr_category(t_eclass* c, const char* attrname, long flags, const char* category)
 * \brief           Sets the category of an attribute.
 * \details         For the moment the category is a dummy member of the attribute. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 * \param category  The category of the attribute
 */
void eclass_attr_category(t_eclass* c, const char* attrname, long flags, const char* category);

/*!
 * \fn          void eclass_attr_order(t_eclass* c, const char* attrname, long flags, const char* order)
 * \brief           Sets the order of an attribute.
 * \details         For the moment the order is a dummy member of the attribute. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 * \param order     The order of the attribute
 */
void eclass_attr_order(t_eclass* c, const char* attrname, long flags, const char* order);

/*!
 * \fn          void eclass_attr_label(t_eclass* c, const char* attrname, long flags, const char* label)
 * \brief           Sets the label of an attribute.
 * \details         The label is the message that will be displayed in the properties window of the object. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 * \param label     The label of the attribute
 */
void eclass_attr_label(t_eclass* c, const char* attrname, long flags, const char* label);

/*!
 * \fn          void eclass_attr_style(t_eclass* c, const char* attrname, long flags, const char* style)
 * \brief           Sets the style of an attribute.
 * \details         The style is how the attribute will be displayed in the properties window of the object. it could be \"onoff\", \"color\", \"number\", \"menu\" or \"entry\". You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 * \param style     The style of the attribute
 */
void eclass_attr_style(t_eclass* c, const char* attrname, long flags, const char* style);

/*!
 * \fn          void eclass_attr_default(t_eclass* c, const char* attrname, long flags, const char* value)
 * \brief           Sets the default value of an attribute.
 * \details         The default value is a string that will be parse into an array of atoms or a array of number if needed. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 * \param value     The default value
 */
void eclass_attr_default(t_eclass* c, const char* attrname, long flags, const char* value);

/*!
 * \fn          void eclass_attr_filter_min(t_eclass* c, const char* attrname, float value)
 * \brief           Sets the minimum value of an attribute.
 * \details         The minimum value is only valid for number attributes. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param value     The minimum value of the attribute
 */
void eclass_attr_filter_min(t_eclass* c, const char* attrname, float value);

/*!
 * \fn          void eclass_attr_filter_max(t_eclass* c, const char* attrname, float value)
 * \brief           Sets the maximum value of an attribute for number attributes.
 * \details         The maximum value is only valid for number attributes. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param value     The maximum value of the attribute
 */
void eclass_attr_filter_max(t_eclass* c, const char* attrname, float value);

/*!
 * \fn          void eclass_attr_step(t_eclass* c, const char* attrname, float value)
 * \brief           Sets the step value of an attribute.
 * \details         The step value is only valid for number attributes and will define the step to increment the value in the properties window. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param value     The maximum value of the attribute
 */
void eclass_attr_step(t_eclass* c, const char* attrname, float value);

/*!
 * \fn          void eclass_attr_save(t_eclass* c, const char* attrname, long flags)
 * \brief           Sets if the attribute should be saved with the object.
 * \details         Only GUI can save their attributes. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 */
void eclass_attr_save(t_eclass* c, const char* attrname, long flags);

/*!
 * \fn          void eclass_attr_paint(t_eclass* c, const char* attrname, long flags)
 * \brief           Sets if the attribute should call the ebox_redraw() function.
 * \details         If the method has been called, when the attribute has changed it call the ebox_redraw function to redraw the object. Only GUI can be repainted. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 */
void eclass_attr_paint(t_eclass* c, const char* attrname, long flags);

/*!
 * \fn          void eclass_attr_invisible(t_eclass* c, const char* attrname, long flags)
 * \brief           Sets if the attribute should be displayed in the properties window. 
 * \details         You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 */
void eclass_attr_invisible(t_eclass* c, const char* attrname, long flags);

/*!
 * \fn          void eclass_attr_accessor(t_eclass* c, const char* attrname, t_err_method getter, t_err_method setter)
 * \brief           Initialize new getter and setter methods for the attributes.
 * \details         By default the attribute is initialized with default getter and setter methods. If you just want to be notify when an attribute has changed, you should prefer to use the notify method. You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param getter    The getter method.
 * \param setter    The setter method.
 */
void eclass_attr_accessor(t_eclass* c, const char* attrname, t_err_method getter, t_err_method setter);

/*!
 * \fn          void eclass_attr_itemlist(t_eclass* c, const char* attrname, long flags, const char* list)
 * \brief           Sets the list available items of an attribute.
 * \details         For example, the font weight attribute offers \"normal\" and \"bold\". You should prefer to use the MACROS.
 * \param c         The t_eclass pointer
 * \param attrname  The attribute name
 * \param flags     The flags of the attribute (dummy)
 * \param list      The list of items.
 */
void eclass_attr_itemlist(t_eclass* c, const char* attrname, long flags, const char* list);

//! @cond
#define calcoffset(x,y) ((long)(&(((x *)0L)->y)))
//! @endcond

//! Macros that create an int attribute
#define CLASS_ATTR_INT(c,name,flags,struct,member)   \
eclass_new_attr_typed(c,name, "int", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a long attribute
#define CLASS_ATTR_LONG(c,name,flags,struct,member)   \
eclass_new_attr_typed(c,name, "long", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a float attribute
#define CLASS_ATTR_FLOAT(c,name,flags,struct,member)  \
eclass_new_attr_typed(c,name, "float", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a double attribute
#define CLASS_ATTR_DOUBLE(c,name,flags,struct,member)  \
eclass_new_attr_typed(c,name, "double", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a rgb attribute
#define CLASS_ATTR_RGB(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "float", 3, 0, flags, calcoffset(struct,member))
//! Macros that create a rgba attribute
#define CLASS_ATTR_RGBA(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "float", 4, 0, flags, calcoffset(struct,member))
//! Macros that create a symbol attribute
#define CLASS_ATTR_SYMBOL(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "symbol", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a atom attribute
#define CLASS_ATTR_ATOM(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "atom", 1, 0, flags, calcoffset(struct,member))
//! Macros that create an int array attribute
#define CLASS_ATTR_INT_ARRAY(c,name,flags,struct,member,size)   \
eclass_new_attr_typed(c,name, "int", size, 0, flags, calcoffset(struct,member))
//! Macros that create a long array attribute
#define CLASS_ATTR_LONG_ARRAY(c,name,flags,struct,member,size)   \
eclass_new_attr_typed(c,name, "long", size, 0, flags, calcoffset(struct,member))
//! Macros that create a float array attribute
#define CLASS_ATTR_FLOAT_ARRAY(c,name,flags,struct,member,size)  \
eclass_new_attr_typed(c,name, "float", size, 0, flags, calcoffset(struct,member))
//! Macros that create a double array attribute
#define CLASS_ATTR_DOUBLE_ARRAY(c,name,flags,struct,member,size)  \
eclass_new_attr_typed(c,name, "double", size, 0, flags, calcoffset(struct,member))
//! Macros that create a symbol array attribute
#define CLASS_ATTR_SYMBOL_ARRAY(c,name,flags,struct,member, size) \
eclass_new_attr_typed(c,name, "symbol", size, 0, flags, calcoffset(struct,member))
//! Macros that create a atom array attribute
#define CLASS_ATTR_ATOM_ARRAY(c,name,flags,struct,member, size) \
eclass_new_attr_typed(c,name, "atom", size, 0, flags, calcoffset(struct,member))
//! Macros that create an int array with a variable size attribute
#define CLASS_ATTR_INT_VARSIZE(c,name,flags,struct,member, size, maxsize)   \
eclass_new_attr_typed(c,name, "int", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a long array with a variable size attribute
#define CLASS_ATTR_LONG_VARSIZE(c,name,flags,struct,member, size, maxsize)  \
eclass_new_attr_typed(c,name, "long", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a float array with a variable size attribute
#define CLASS_ATTR_FLOAT_VARSIZE(c,name,flags,struct,member, size, maxsize) \
eclass_new_attr_typed(c,name, "float", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a double array with a variable size attribute
#define CLASS_ATTR_DOUBLE_VARSIZE(c,name,flags,struct, member, size, maxsize)  \
eclass_new_attr_typed(c,name, "double", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a symbol array with a variable size attribute
#define CLASS_ATTR_SYMBOL_VARSIZE(c,name,flags,struct, member, size, maxsize) \
eclass_new_attr_typed(c,name, "symbol", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a atom array with a variable size attribute
#define CLASS_ATTR_ATOM_VARSIZE(c,name,flags,struct, member, size, maxsize) \
eclass_new_attr_typed(c,name, "atom", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))

//! CEAMMC
//! Macros that creates virtual invisible attribute only with getter and setter access
#define CLASS_ATTR_VIRTUAL(c, name, getter, setter) \
    eclass_new_attr_typed(c, name, "float", 1, 0, 0, 0); \
    eclass_attr_invisible(c, name, 0);\
    eclass_attr_accessor(c, name, (t_err_method)getter, (t_err_method)setter)

//! Macros that define the category of the attributes
#define CLASS_ATTR_CATEGORY(c,name,flags,categoryname)  eclass_attr_category(c,name,flags,categoryname)
//! Macros that define the order of the attributes
#define CLASS_ATTR_ORDER(c,name,flags,order)            eclass_attr_order(c,name,flags,order)
//! Macros that define the label of the attributes
#define CLASS_ATTR_LABEL(c,name,flags,label)            eclass_attr_label(c,name,flags,label)
//! Macros that define the style of the attributes
#define CLASS_ATTR_STYLE(c,name,flags,style)            eclass_attr_style(c,name,flags,style)
//! Macros that define the default value of the attributes
#define CLASS_ATTR_DEFAULT(c,name,flags,val)            eclass_attr_default(c,name,flags,val)
//! Macros that define the minimum value of the attributes
#define CLASS_ATTR_FILTER_MIN(c, name, value)           eclass_attr_filter_min(c, name, value)
//! Macros that define the maximum value of the attributes
#define CLASS_ATTR_FILTER_MAX(c, name, value)           eclass_attr_filter_max(c, name, value)
//! Macros that define the minimum and maximmum values of the attributes
#define CLASS_ATTR_FILTER_CLIP(c, name, minval, maxval) eclass_attr_filter_min(c, name, minval); eclass_attr_filter_max(c, name, maxval);
//! Macros that define the step value of the attributes
#define CLASS_ATTR_STEP(c, name, value)                 eclass_attr_step(c, name, value)
//! Macros that define the save behavior of the attributes
#define CLASS_ATTR_SAVE(c,name,flags)                   eclass_attr_save(c,name,flags)
//! Macros that define the paint behavior of the attributes
#define CLASS_ATTR_PAINT(c,name,flags)                  eclass_attr_paint(c,name,flags)
//! Macros that define the visible behavior of the attributes
#define CLASS_ATTR_INVISIBLE(c,name,flags)              eclass_attr_invisible(c,name,flags)
//! Macros that define the setter and getter of the attributes
#define CLASS_ATTR_ACCESSORS(c,name,getter,setter)      eclass_attr_accessor(c,name,(t_err_method)getter,(t_err_method)setter)
//! Macros that define the items list of the attributes
#define CLASS_ATTR_ITEMS(c,name,flags, list)            eclass_attr_itemlist(c,name,flags, list)
//! Macros that define the deault value, save and paint bbehavior of the attributes
#define CLASS_ATTR_DEFAULT_SAVE_PAINT(c,attrname,flags,parsestr) \
{ CLASS_ATTR_DEFAULT(c,attrname,flags,parsestr); CLASS_ATTR_SAVE(c,attrname,flags); CLASS_ATTR_PAINT(c,attrname,flags); }

/** @} */

#endif



