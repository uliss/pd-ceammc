//
//  tl_object_common.hpp
//  pd_ext
//
//  Created by Alex Nadzharov on 09/12/16.
//
//  Common object for all timeline tl.* objects

/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.*/


#ifndef tl_object_common_h
#define tl_object_common_h

#include <stdlib.h>
#include <vector>
#include "m_pd.h"
#include "ceammc_gui.hpp"

//#include "g_all_guis.h"

#pragma mark pd objects

//TEMP


//struct tl_bang_object {
//    t_object x_obj;
//    
//    t_outlet *out1;
//};

#pragma mark -

//temporary
//TODO types for all library
typedef struct
{
    int size;
    t_atom *data;
} tl_t_list;

typedef void(*tl_ui_action)(t_object*);//, tl_t_list) ;     //bang action + optional data. yet removed
typedef void(*tl_ui_drawcue)(t_object*) ;                   //temporary


typedef struct{
    float x_pos;
    int cue_number;
    float cue_time;     //in ms
    bool use_time;
    t_object *ui_obj;
} tl_t_cue;


typedef struct
{
    float x_pos;
    t_object *ui_obj;
} tl_t_ui_object;

typedef struct
{
    
}tl_t_action;

typedef struct
{
    tl_ui_action ui_action;
    //tl_ui_drawcue drawcue;
} tl_t_methods;


#pragma mark -


EXTERN tl_t_list tll_list_createempty();

EXTERN void tll_cue_add(t_object *x, float x_pos);

EXTERN void tll_cue_update_all_pos();
EXTERN void tll_cue_update_pos(t_object *x, float x_pos);

EXTERN void tll_cue_delete(t_object *x);

EXTERN void tll_cue_dump();

EXTERN int tll_cue_getnumber(t_object* obj);

///////

EXTERN void tll_ui_add(t_object *x, float x_pos);

EXTERN void tll_ui_update_all_pos();
EXTERN void tll_ui_update_pos(t_object *x, float x_pos);

EXTERN void tll_ui_dump();

///////

EXTERN void tll_perform_actions_for_cue(int cue_number);

///////

EXTERN void tll_set_ui_action (tl_ui_action method);
EXTERN void tll_set_ui_drawcue (tl_ui_drawcue method);





#endif