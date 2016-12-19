//
//  new_c_gui.h
//  pd_ext
//
//  Created by Alex Nadzharov on 15/12/16.
//
//

#ifndef new_c_gui_h
#define new_c_gui_h

#include "m_pd.h"
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "ceammc_draw_wrappers.h"
//#include "ceammc_named_atom.h"



//MOVE

//#pragma gui extra

//based on that:
//https://github.com/CICM/CicmWrapper
//Copyright (C) 2013-2015 Pierre Guillot - CICM - Universite Paris 8
//The Cream Library in under the BSD2 License.

//std::string  ui_x_obj_cnv_id (t_canvas *c1)
//{
//
//    std::string ret;
//    char buffer[MAXPDSTRING];
//    
//    sprintf(buffer,".x%lx.c", (long unsigned int) c1);
//    ret = buffer;
//    return ret;
//    
//    
//}
//std::string ui_x_obj_ui_id(t_object *x , std::string cls_name)
//{
//    
//    std::string ret;
//    char buffer[MAXPDSTRING];
//    
//    sprintf(buffer,"#%s%lx", cls_name.c_str(), (long unsigned int)x);
//    ret = buffer;
//    return ret;
//    
//}

//void ui_x_bind(t_object *x, std::string cls_name, t_canvas *c1)
//{
//   
//    
//    // object symbol name
//    
////    sys_vgui("bind %s <Button-3> {+pdsend {%s mousedown %%x %%y %i}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name, EMOD_RIGHT);
////    sys_vgui("bind %s <Button-2> {+pdsend {%s mousedown %%x %%y %i}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name, EMOD_RIGHT);
////    sys_vgui("bind %s <Button-1> {+pdsend {%s mousedown %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
////    sys_vgui("bind %s <ButtonRelease> {+pdsend {%s mouseup %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
//    
////    sys_vgui("bind %s <Motion> {+pdsend {%s mousemove %%x %%y %%s}}\n", ui_x_obj_cnv_id(c1).c_str(), ui_x_obj_ui_id(x,cls_name).c_str() );
////    
////    printf(" ** bind: %s %s \n", ui_x_obj_cnv_id(c1).c_str(), ui_x_obj_ui_id(x,cls_name).c_str());
////
////    sys_vgui("bind %s <Enter> {+pdsend {%s mouseenter}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
////    sys_vgui("bind %s <Leave> {+pdsend {%s mouseleave}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
//    
//}


#pragma mark UI defines

#define UI_fun(x) template<> void cm_gui_object<x>
#define UI_funp(x) template<> void* cm_gui_object<x>
#define UI_basefun cm_gui_object<cm_gui_base_pd_object>
#define UI_Prop cm_gui_properties *x  = instances[(t_object *)z];
#define UI_Pf x->ui_property_get_float
#define UI_Ps x->ui_property_get_c_str
#define UI_ draw_wrappers::
#define UI_Pset x->ui_property_set
#define UI_x ((t_text*) &(((cm_gui_base_pd_object*)z)->x_gui))->te_xpix
#define UI_y ((t_text*) &(((cm_gui_base_pd_object*)z)->x_gui))->te_ypix

#define UI_Setup  gui_set_canvas(glist); gui_set_object((t_object*)z);


#pragma mark -

typedef std::map<std::string,t_atom> cm_gui_properties_;

class  cm_gui_properties: public cm_gui_properties_
{
public:
    
    t_atom ui_property_get(std::string name)
    {
        t_atom ret = (*this)[name];
        
        return ret;
    }
    
    t_float ui_property_get_float(std::string name)
    {
        //printf("property get float\n");
        
        if ((*this)[name].a_type == A_FLOAT)
        {
            t_float ret = (*this)[name].a_w.w_float;
            
            return ret;
        }
        else
        {
            post("ERR: bad float value");
            return 0;
        }
    }
    
    char* ui_property_get_c_str(std::string name)
    {
        //printf("property get cstr %s\n", name.c_str());
        
        if ((*this)[name].a_type == A_SYMBOL)
        {
            char* ret = (*this)[name].a_w.w_symbol->s_name;
            
            return ret;
        }
        else
        {
            post("ERR: bad symbol value");
            printf ("property get cstr err\n");
            char* no = "<error>";
            return no;
        }
    }
    
    void ui_property_set(std::string name, t_atom value)
    {
        (*this)[name] = value;
    }
    
    void ui_property_set(std::string name, std::string value)
    {
        t_atom a;
        a.a_w.w_symbol = gensym(value.c_str());
        a.a_type = A_SYMBOL;
        (*this)[name] = a;
    }
    
    void ui_property_set(std::string name, float value)
    {
        t_atom a;
        a.a_w.w_float = value;
        a.a_type = A_FLOAT;
        (*this)[name] = a;
    }
    
#pragma mark -
    void log()
    {
        
        cm_gui_properties::iterator it;
        
        for (it = this->begin(); it != this->end(); ++it)
        {
            (it->second.a_type == A_SYMBOL)?
            printf("[ %s ] : %s \n", it->first.c_str(), it->second.a_w.w_symbol->s_name )
            :
            printf("[ %s ] : %f \n", it->first.c_str(), it->second.a_w.w_float )
            ;
        }
        
        printf("\n");
    }
};


struct cm_gui_base_pd_object
{
    t_object x_gui;
    t_inlet **inlets;
    t_inlet **outlets;
    int inlet_count;
    int oulet_count;
    t_atomtype *inlet_types;
    t_atomtype *outet_types;
    
};

#pragma mark -------

typedef std::map<t_object*, cm_gui_properties*> cm_gui_instances;


template <typename U>
class cm_gui_object {
    
public:
#pragma mark static - def
    
    static t_class* pd_class;
    static cm_gui_instances instances;

    static cm_gui_properties *ui_default_properties;
    
    static t_widgetbehavior widget;
    
    static std::string class_name;

    

    static void ui_properties_init()
    {
        //printf("**property init default [%lu]\n", (long)this);
        
        if (ui_default_properties)
        {
            printf("p del\n");
            //delete this->ui_properties;
            delete ui_default_properties;
        }
        
        ui_default_properties = new cm_gui_properties;
        
        //ui_default_properties->ui_property_set("object_name", "ceammc.gui_object");
        
        ui_default_properties->ui_property_set("width", 45);
        ui_default_properties->ui_property_set("height", 15);
        
        ui_default_properties->ui_property_set("font", "Helvetica");
        ui_default_properties->ui_property_set("font_size", 14.);
        
        ui_default_properties->ui_property_set("color_bg", "#FFFFFF");
        ui_default_properties->ui_property_set("color_text", "#000000");
        ui_default_properties->ui_property_set("color_line", "#00C0FF");
        
        ui_default_properties->ui_property_set("send_id", "no_send");
        ui_default_properties->ui_property_set("receive_id", "no_receive");
        
        ui_default_properties->ui_property_set("label", "CEAMMC_gui_template");
        
        ui_default_properties->ui_property_set("label_x", 7);
        ui_default_properties->ui_property_set("label_y", 7);
        
        ui_default_properties->ui_property_set("_selected", 0);
        
        ui_properties_init_ext(ui_default_properties);
        
        printf( "** done\n");
        
        printf("********** init\n");
        ui_default_properties->log();
        
    }
    
    
    

    
#pragma mark -
#pragma mark 'extensions'
    
    static void new_ext(t_object *x, t_symbol *s, int argc, t_atom *argv)
    {
        
    }
    
    static void load_ext(t_object *x)
    {
        
    }
    
    static void free_ext(t_object *x)
    {
        
    }
    
    static void ui_properties_init_ext(cm_gui_properties *def_p)
    {
        
    }
    
#pragma mark pd object instance
    
    static void *new_method(t_symbol *s, int argc, t_atom *argv)
    {
        printf("new method %lu",(long)cm_gui_object<U>::pd_class);
        
        t_object* z = reinterpret_cast<t_object*>(pd_new((t_class*)cm_gui_object<U>::pd_class));
        
        //U *x = (U*)z;
        
        cm_gui_properties *inst_prop = new cm_gui_properties;
        *inst_prop = *(cm_gui_object<U>::ui_default_properties);
        instances[z] = (inst_prop);
        
        //t_text *t = (t_text*) &(((U*)z)->x_gui);
        //printf("-new coords %d %d\n", t->te_xpix, t->te_ypix);
        
        cm_gui_object<U>::load_method(z, argv);
        cm_gui_object<U>::new_ext(z, s, argc, argv);
        
        
//        pd_bind((t_pd*)cm_gui_object<U>::pd_class, gensym(cm_gui_object<U>::class_name.c_str()));
//        ui_x_bind(z, cm_gui_object<U>::class_name.c_str(), canvas_getcurrent() );
        
        return static_cast<void*>(z);
    }
    


    
    static void free_method(t_object *x)
    {
        cm_gui_object<U>::free_ext(x);
        
        instances.erase(x);
        
        printf("free");
        
    }
    
    static bool load_method (t_object *z,t_atom *values)
    {
        printf("** property load [%lu]\n", (long)z);
        int count = (int)(instances[z]->size());
        int i=0;
        
        printf("count: %i\n", count);
        //ceammc_gui::e_atoms(values, count);
        
        std::map<std::string, t_atom>::iterator it;
        
        for (it = (*instances[z]).begin(); it != (*instances[z]).end(); ++it)
        {
            if (i==count) break;
            
            t_atom a = values[i];
            if (a.a_type != it->second.a_type)
            {
                printf("ERR broken object data\n");
                
                return false;
            }
            else
            {
                it->second = a;
            }
            
            i++;
            
        }
        
        printf( "** loaded %i\n",i);
        
        printf("********** load\n");
        //ceammc_gui::e_properties((t_object*)this);
        instances[z]->log();
        
        return true;
    }

    
    static void save_method(t_gobj *z, t_binbuf *b)
    {
        //U *x = (U*)((cm_gui_base_pd_object*)z)->x_gui;
        
        cm_gui_properties *x  = instances[(t_object *)z];
        
        char c_sym[] = "s";
        char c_int[] = "i";
        char c_sc[] = ";";
        
        binbuf_addv(b,c_sym, gensym("#X"));
        binbuf_addv(b,c_sym, gensym("obj"));
        
        
        binbuf_addv(b, c_int, UI_x);
        binbuf_addv(b, c_int, UI_y);
        
        //TODO wrap in quotes
        binbuf_addv(b, c_sym, gensym(cm_gui_object<U>::class_name.c_str()));
        
        std::map<std::string,t_atom>::iterator it;
        for (it = (*x).begin(); it!= (*x).end(); ++it)
        {
            binbuf_add(b, 1, &it->second);
        }
        
        binbuf_addv(b, c_sc);
        
        printf("********** save\n");
        x->log();
        
        char *bchar; int l;
        binbuf_gettext(b, &bchar, &l);
        //printf("data: %s\n\n", bchar);
        
        
    }
    
    
#pragma mark -
#pragma mark standard widget methods
    
    
    static void w_draw(t_gobj *z, t_glist *glist)
    {
        //printf("w_draw stub");
    }
    
    static void w_erase(t_gobj *z, t_glist *glist)
    {
        //printf("w_erase stub");
    }
    
    static void w_select(t_gobj *z, t_glist *glist, int selected)
    {
        UI_Setup
        UI_Prop
        
        UI_Pset("_selected", selected);
        
        cm_gui_object<U>::w_erase(z, glist);
        cm_gui_object<U>::w_draw(z, glist);
        
        printf("sel %f\n", UI_Pf("_selected"));
    }
    
    static void w_vis(t_gobj *z, t_glist *glist,int vis)
    {
        if (vis)
            cm_gui_object<U>::w_draw(z, glist);
        else
            cm_gui_object<U>::w_erase(z, glist);
        
    }
    
    static void w_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2)
    {
        
        UI_Setup
        UI_Prop
        
        *x1 = UI_x;
        *y1 = UI_y;
        *x2 = *x1 + UI_Pf("width");
        *y2 = *y1 + UI_Pf("height");
        
        
    }
    
    
    static void w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
    {
        UI_x += dx;
        UI_y += dy;
    }
    

    
#pragma mark extended ui methods
static void wx_mousemove(t_object* x, t_symbol* s, int argc, t_atom* argv)
    {
        printf("!mousemove\n");
    }
   

#pragma mark -
    
public:
    void setup(std::string _class_name)
    {
        t_class *cl;

        cl = class_new(gensym(_class_name.c_str()),reinterpret_cast<t_newmethod>(cm_gui_object<U>::new_method), reinterpret_cast<t_method>(cm_gui_object<U>::free_method), sizeof(U), CLASS_PATCHABLE, A_GIMME,0);
        
        printf ("cl %lu\n",(long)cl);
        
        cm_gui_object<U>::class_name = _class_name;
        cm_gui_object<U>::ui_properties_init();
        
        //widget
        widget.w_visfn = &cm_gui_object<U>::w_vis;
        widget.w_displacefn = &cm_gui_object<U>::w_displace;
        widget.w_getrectfn = &cm_gui_object<U>::w_getrect;
        widget.w_selectfn = &cm_gui_object<U>::w_select;
        
        class_setwidget(cl, &widget);
        
        class_setsavefn(cl, cm_gui_object<U>::save_method);
        
        cm_gui_object<U>::pd_class = cl;
        
        //cm_gui_object<U>::w_testfunc() ;
        
        
        class_addmethod(cl, reinterpret_cast<t_method>(cm_gui_object<U>::wx_mousemove), gensym("mousemove"), A_GIMME,0);
        
        
        
        
    }
    
    void setup_io(std::string _class_name, t_atomtype *ins, t_atomtype *outs)
    {
        this->setup(_class_name);
        
        
    }
    
};



template <typename U>
t_class* cm_gui_object<U>::pd_class;

template <typename U>
cm_gui_properties *cm_gui_object<U>::ui_default_properties;

template <typename U>
cm_gui_instances cm_gui_object<U>::instances;

template <typename U>
t_widgetbehavior cm_gui_object<U>::widget;
        
                    template <typename U>
                    std::string cm_gui_object<U>::class_name;

#endif /* new_c_gui_h */
