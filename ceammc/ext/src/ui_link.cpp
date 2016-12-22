//
//  ui_link.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 22/12/16.
//
//

#include <stdio.h>

#include "ceammc_gui_cicm.h"

#include "m_imp.h"  /* FIXME need access to c_externdir... */
//#include "g_canvas.h"

//#include "s_path.c"

#include <string>
#include <iostream>

struct ui_link : cm_gui_base_pd_object
{
    t_ebox x_gui;
    
    t_etext *t_e;
    t_efont *t_ef;
    
    t_symbol *link;
    t_symbol *title;
    
   // std::string display;
    
};

//TEMP
template <typename T> std::string to_str(const T& t) {
    std::ostringstream os;
    os<<t;
    return os.str();
}





//////

UI_fun(ui_link)::wx_paint(t_object *z, t_object *view)
{
    //UI_Prop
    
    t_symbol *bgl = gensym("background_layer");
    float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    
    ui_link *zx = (ui_link*)z;
    
    if(g)
    {
        egraphics_set_color_hex(g, gensym("#E0E0E0"));
        
        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
        egraphics_fill(g);
        
        //int argc;
        //t_atom *argv;
        //eclass_attr_getter(z, gensym("link"),&argc , &argv);
        
//        if (argc)
//        {
//            if (argv[0].a_type == A_SYMBOL)
//            {
//                zx->display = argv[0].a_w.w_symbol->s_name;
//            }
//        }
        //zx->display.c_str()
        
        etext_layout_set(zx->t_e, zx->title->s_name, zx->t_ef, rect.width/2, rect.height/2, rect.width, rect.height, ETEXT_CENTER, ETEXT_JLEFT, ETEXT_WRAP);
        
        etext_layout_draw(zx->t_e, g);
        
        
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
    
    
}

UI_fun(ui_link)::wx_mousedown_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
{
    ui_link *zx = (ui_link*)z;
//    
//    
//    t_symbol *dir = canvas_getdir((t_glist*)view);
//    
//    char final_name[FILENAME_MAX];
//    sys_expandpathelems(zx->display.c_str(), final_name);
    
    //printf("file %s\n", zx->display.c_str() );
    
    sys_vgui("open_file [file normalize {%s}] \n",
             zx->link->s_name );
    
    
    
}

UI_fun(ui_link)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
{
    ui_link *zx = (ui_link*)z;
    zx->t_e = etext_layout_create();
    
    zx->t_ef = efont_create(gensym("Helvetica"), gensym(""), gensym("normal"), 12);
    
    
}

UI_fun(ui_link)::free_ext(t_object *z)
{
    ui_link *zx = (ui_link*)z;
    efont_destroy(zx->t_ef);
    
}

UI_fun(ui_link)::init_ext(t_eclass *z)
{
    CLASS_ATTR_DEFAULT (z, "size", 0, "120. 15.");
    
    CLASS_ATTR_SYMBOL(z, "link", 0, ui_link, link);
    CLASS_ATTR_DEFAULT(z, "link", 0, "<no link>");
    CLASS_ATTR_LABEL(z, "link", 0, "Link");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "link", 0, "<no link>");
    
    CLASS_ATTR_SYMBOL(z, "title", 0, ui_link, title);
    CLASS_ATTR_DEFAULT(z, "title", 0, "<no title>");
    CLASS_ATTR_LABEL(z, "title", 0, "Title");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "title", 0, "<no title>");
}

extern "C" void setup_ui0x2elink()
{
    cm_gui_object<ui_link> class1;
    class1.setup_noin("ui.link");
}