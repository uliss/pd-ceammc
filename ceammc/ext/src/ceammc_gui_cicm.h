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

//#include "ceammc_draw_wrappers.h"
//#include "ceammc_named_atom.h"

//#include "../../extra/CicmWrapper/Sources/cicm_wrapper.h"

//TEMP

extern "C" {
    
#include "../../extra/CicmWrapper/Sources/eclass.c"
#include "../../extra/CicmWrapper/Sources/ecommon.c"
#include "../../extra/CicmWrapper/Sources/eobj.c"
#include "../../extra/CicmWrapper/Sources/ebox.c"
#include "../../extra/CicmWrapper/Sources/epopup.c"
#include "../../extra/CicmWrapper/Sources/egraphics.c"
#include "../../extra/CicmWrapper/Sources/epdmax.h"
    
}




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
            //char no[] = "";
            return nullptr;
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
    t_ebox b_box;
    //    t_inlet **inlets;
    //    t_inlet **outlets;
    //    int inlet_count;
    //    int oulet_count;
    //    t_atomtype *inlet_types;
    //    t_atomtype *outet_types;
    
};

#pragma mark -------

typedef std::map<t_object*, cm_gui_properties*> cm_gui_instances;


template <typename U>
class cm_gui_object {
    
public:
#pragma mark static - def
    
    static t_eclass* pd_class;
    static cm_gui_instances instances;
    
    static cm_gui_properties *ui_default_properties;
    
    //static t_ewidget widget;
    
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
    
    static void new_ext(t_object *x, t_symbol *s, int argcl, t_atom *argv)
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
    
    static void *new_method(t_symbol *s, int argcl, t_atom *argv)
    {
        printf("new instance (%lu) \n",(long)cm_gui_object<U>::pd_class);
        
        t_object* z = (t_object*)eobj_new(cm_gui_object<U>::pd_class);
        
        //reinterpret_cast<t_object*>(pd_new((t_class*)cm_gui_object<U>::pd_class));
        
        //U *x = (U*)z;
        
        cm_gui_properties *inst_prop = new cm_gui_properties;
        *inst_prop = *(cm_gui_object<U>::ui_default_properties);
        instances[z] = (inst_prop);
        
        printf("...\n");
        t_binbuf* d = binbuf_via_atoms(argcl,argv);
        ebox_new((t_ebox *)z, 0 | EBOX_GROWINDI);  //| EBOX_GROWLINK
        
        
        
        if (z && d)
        {
            ebox_attrprocess_viabinbuf(z, d);}
        
        ebox_ready((t_ebox *)z);
        
        //t_text *t = (t_text*) &(((U*)z)->x_gui);
        //printf("-new coords %d %d\n", t->te_xpix, t->te_ypix);
        
        cm_gui_object<U>::load_method(z, argv);
        cm_gui_object<U>::new_ext(z, s, argcl, argv);
        
        
        //        pd_bind((t_pd*)cm_gui_object<U>::pd_class, gensym(cm_gui_object<U>::class_name.c_str()));
        //        ui_x_bind(z, cm_gui_object<U>::class_name.c_str(), canvas_getcurrent() );
        
        printf("new instance done\n");
        return static_cast<void*>(z);
    }
    
    
    
    
    static void free_method(t_object *x)
    {
        cm_gui_object<U>::free_ext(x);
        
        instances.erase(x);
        
        ebox_free((t_ebox *)x);
        
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
        
        
        //        binbuf_addv(b, c_int, UI_x);
        //        binbuf_addv(b, c_int, UI_y);
        
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
    
    
    //    static void w_draw(t_gobj *z, t_glist *glist)
    //    {
    //        printf("w_draw stub");
    //    }
    //
    //    static void w_erase(t_gobj *z, t_glist *glist)
    //    {
    //        printf("w_erase stub");
    //    }
    
    static void w_select(t_gobj *z, t_glist *glist, int selected)
    {
        //UI_Setup
        UI_Prop
        
        UI_Pset("_selected", selected);
        
        //        cm_gui_object<U>::w_erase(z, glist);
        //        cm_gui_object<U>::w_draw(z, glist);
        
        printf("sel %f\n", UI_Pf("_selected"));
    }
    
    //    static void w_vis(t_gobj *z, t_glist *glist,int vis)
    //    {
    //        if (vis)
    //            cm_gui_object<U>::w_draw(z, glist);
    //        else
    //            cm_gui_object<U>::w_erase(z, glist);
    //
    //    }
    
    //    static void w_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2)
    //    {
    //
    //        //UI_Setup
    //        UI_Prop
    ////
    ////        *x1 = UI_x;
    ////        *y1 = UI_y;
    //        *x2 = *x1 + UI_Pf("width");
    //        *y2 = *y1 + UI_Pf("height");
    //
    //
    //    }
    
    
    //    static void w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
    //    {
    ////        UI_x += dx;
    ////        UI_y += dy;
    //    }
    
    
    
#pragma mark extended ui methods
    
    static void wx_paint(t_object *x, t_object *view)
    {
        t_symbol *bgl = gensym("background_layer");
        float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)x, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)x, bgl, rect.width, rect.height);
        if(g)
        {
            
            //            size = rect.width * 0.5;
            //
            //            egraphics_set_color_hex(g, gensym("#00C0FF"));
            //
            //            egraphics_circle(g, floor(size + 0.5), floor(size + 0.5), size * 0.9);
            //            egraphics_fill(g);
            //            ebox_end_layer((t_ebox*)x, bgl);
        }
        
        ebox_paint_layer((t_ebox *)x, bgl, 0., 0.);
    }
    
    static void wx_mousemove(t_object* x, t_symbol* s, int argcl, t_atom* argv)
    {
        printf("!mousemove\n");
    }
    
    
#pragma mark -
    
public:
    void setup(std::string _class_name)
    {
        t_eclass *cl;
        
        cl = eclass_new(_class_name.c_str(),(method)cm_gui_object<U>::new_method, (method)&cm_gui_object<U>::free_method, sizeof(U), CLASS_PATCHABLE, A_GIMME,0);
        
        printf("init\n");
        
        if (cl)
        {
            eclass_guiinit(cl, 0);
            
            
            cm_gui_object<U>::class_name = _class_name;
            cm_gui_object<U>::ui_properties_init();
            
            //widget
            
            //TODO methods
            //                    cl->c_widget.w_visfn = &cm_gui_object<U>::w_vis;
            //                    cl->c_widget.w_displacefn = &cm_gui_object<U>::w_displace;
            //                    cl->c_widget.w_getrectfn = &cm_gui_object<U>::w_getrect;
            //                    cl->c_widget.w_selectfn = &cm_gui_object<U>::w_select;
            
            
            
            // We intialize the attribute of the t_bang.
            // All the GUI classes has font attributes but we don't need them for the bang classe so we mark them invisible.
            //            CLASS_ATTR_INVISIBLE            (cl, "fontname", 1);
            //            CLASS_ATTR_INVISIBLE            (cl, "fontweight", 1);
            //            CLASS_ATTR_INVISIBLE            (cl, "fontslant", 1);
            //            CLASS_ATTR_INVISIBLE            (cl, "fontsize", 1);
            //            // All the GUI classes has a size attribute, we just set up the default value.
            //            CLASS_ATTR_DEFAULT              (cl, "size", 0, "16. 16.");
            
                        // We create a new t_rgba attribute that refers to the b_color_background member of the t_bang and that will match to
                        // "bgcolor". The user will be able to change the background color with the "bgcolor" message.
                        //CLASS_ATTR_RGBA                 (cl, "bgcolor", 0, U, b_color_background);
                        // We set up the label that will be displayed in the properties window of the object for the attribute.
                        CLASS_ATTR_LABEL                (cl, "bgcolor", 0, "Background Color");
                        // We set up the order of the attribute in the properties window (this is unused for the moment).
                        CLASS_ATTR_ORDER                (cl, "bgcolor", 0, "1");
                        // We set up the the default value of the color. This macro also defines that the attribute will automatically call ebox_redraw when its value has changed and that its value will be saved with the patcher.
                        CLASS_ATTR_DEFAULT_SAVE_PAINT   (cl, "bgcolor", 0, "0.75 0.75 0.75 1.");
                        // We set up the that the attribute should be displayed as a color slector in the properties window.
                        CLASS_ATTR_STYLE                (cl, "bgcolor", 0, "color");
                        // We do the same thing for the border color and the bang color.
                        //CLASS_ATTR_RGBA                 (cl, "bdcolor", 0, U, b_color_border);
                        CLASS_ATTR_LABEL                (cl, "bdcolor", 0, "Border Color");
                        CLASS_ATTR_ORDER                (cl, "bdcolor", 0, "2");
                        CLASS_ATTR_DEFAULT_SAVE_PAINT   (cl, "bdcolor", 0, "0.5 0.5 0.5 1.");
                        CLASS_ATTR_STYLE                (cl, "bdcolor", 0, "color");
            
                        //CLASS_ATTR_RGBA                 (cl, "bacolor", 0, U, b_color_bang);
                        CLASS_ATTR_LABEL                (cl, "bacolor", 0, "Bang Color");
                        CLASS_ATTR_ORDER                (cl, "bacolor", 0, "3");
                        CLASS_ATTR_DEFAULT_SAVE_PAINT   (cl, "bacolor", 0, "0. 0. 0. 1.");
                        CLASS_ATTR_STYLE                (cl, "bacolor", 0, "color");
            
            eclass_addmethod(cl, (method)(&cm_gui_object<U>::wx_paint), ("paint"), A_GIMME,0);
            
            eclass_addmethod(cl, (method)(&cm_gui_object<U>::wx_mousemove), ("mousemove"), A_GIMME,0);
            
            //
            //cl->c_widget.w_save = &cm_gui_object<U>::save_method;
            
            CLASS_ATTR_DEFAULT (cl, "size", 0, "45. 15.");
            
            
            
            cm_gui_object<U>::pd_class = cl;
            
            //eclass_register(CLASS_BOX, cl);
            
            
            printf("gui init (%lu)\n",(long)cl);
            
            
        }
        
        printf("init done\n");
        
    }
    
    void setup_io(std::string _class_name, t_atomtype *ins, t_atomtype *outs)
    {
        this->setup(_class_name);
        
        
    }
    
};



template <typename U>
t_eclass* cm_gui_object<U>::pd_class;

template <typename U>
cm_gui_properties *cm_gui_object<U>::ui_default_properties;

template <typename U>
cm_gui_instances cm_gui_object<U>::instances;

//template <typename U>
//t_ewidget cm_gui_object<U>::widget;

template <typename U>
std::string cm_gui_object<U>::class_name;

#endif /* new_c_gui_h */
