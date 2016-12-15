//
//  tl_bang2.h
//  pd_ext
//
//  Created by Alex on 13/12/16.
//
//

#ifndef tl_bang2_h
#define tl_bang2_h

#include "ceammc_gui.cpp"


#include <stdlib.h>
//#include <m_pd.h>

class tl_bang2:public ceammc_gui{
public:
    static t_class *ceammc_gui_pd_class;
    virtual t_class* get_pd_class();
    virtual void set_pd_class(t_class* c1);
    
    //virtual t_newmethod get_pd_class_new();
    static void *pd_class_new1(t_symbol *s, int argc, t_atom *argv);
    
    t_widgetbehavior *w_ ;
    
        //    void w_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2) ;
        static void w_displace(t_gobj *z, t_glist *glist, int dx, int dy) ;
        //virtual  void w_delete(t_gobj *z, t_glist *glist) ;
        static  void w_draw(t_gobj *z, t_glist *glist);
        static  void w_erase(t_gobj *z, t_glist *glist);
        //int w_click(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit);
        static void w_select(t_gobj *z, t_glist *glist, int selected);
   
    static void w_vis(t_gobj *z, t_glist *glist,int vis);

    
};


class tl_bang2_object : public ceammc_gui_object
{
public:
    virtual long size(){return sizeof(tl_bang2_object);}
    
    virtual void ui_property_init();
    
    
//    //    void w_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2) ;
//    virtual void w_displace(t_gobj *z, t_glist *glist, int dx, int dy) ;
//    //virtual  void w_delete(t_gobj *z, t_glist *glist) ;
//    virtual  void w_draw(t_gobj *z, t_glist *glist);
//    virtual  void w_erase(t_gobj *z, t_glist *glist);
//    //int w_click(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit);
//    virtual void w_select(t_gobj *z, t_glist *glist, int selected);
    
    virtual void pd_instance_init(t_object *obj);
    
    t_outlet *outlet1;
    
    virtual t_newmethod get_pd_class_new();
    
};

#endif /* tl_bang2_h */
