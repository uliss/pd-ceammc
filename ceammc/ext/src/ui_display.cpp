//
//  ui_display.cpp
//  pd_ext
//
//  Created by Alex on 21/12/16.
//
//

#include <stdio.h>

#include "lib/ceammc_gui.h"

#include <string>
#include <iostream>

struct ui_display : ceammc_gui::base_pd_object
{
    t_ebox x_gui;
    
    int argc;
    t_atom *argv;
    
    t_etext *t_e;
    t_efont *t_ef;
    
    t_clock *t_c;
    
    bool bang;
    
    std::string display;
    
};

//TEMP
template <typename T> std::string to_str(const T& t) {
    std::ostringstream os;
    os<<t;
    return os.str();
}

UI_fun(ui_display)::wx_paint(t_object *z, t_object *view)
{
    //UI_Prop
    
    t_symbol *bgl = gensym("background_layer");
    float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    
    ui_display *zx = (ui_display*)z;
    
    if(g)
    {
        egraphics_set_color_hex(g, gensym(zx->bang?"#00C0FF":"#E0E0E0"));
        
        //egraphics_set_color_hex(g, gensym("#E0E0E0"));
        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
        egraphics_fill(g);
        
        
        //zx->t_e->c_text = gensym(zx->display.c_str());
        
        //egraphics_set_color_hex(g, gensym(zx->bang?"#00C0FF":"#000000"));
        
        etext_layout_set(zx->t_e, zx->display.c_str(), zx->t_ef, rect.width/2, rect.height/2, rect.width, rect.height, ETEXT_CENTER, ETEXT_JLEFT, ETEXT_WRAP);
        
        etext_layout_draw(zx->t_e, g);
        
        //printf("draw %s\n", zx->display.c_str());
        
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
    
    
}

UI_fun(ui_display)::m_anything(t_object *z, t_symbol *s, int argc, t_atom *argv)
{
    printf("anything\n");
    ui_display *zx = (ui_display*)z;
    
    
    
    zx->display = "";
    zx->display = zx->display + s->s_name + " ";
    
    for (int i=0;i<argc;i++)
    {
        //TEMP
        if (argv[i].a_type==A_FLOAT)
        {
            zx->display = zx->display + to_str(argv[i].a_w.w_float);
        }
        
        if (argv[i].a_type==A_SYMBOL)
        {
            zx->display = zx->display + to_str(argv[i].a_w.w_symbol->s_name);
        }
        
        zx->display = zx->display + " ";
        
    }
    
    zx->bang = true;
    
    clock_delay(zx->t_c, 100);
    
    ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
    
}

UI_fun(ui_display)::m_list(t_object *z, t_symbol *s, int argc, t_atom *argv)
{
    ceammc_gui::object<ui_display>::m_anything(z, &s_list, argc, argv);
}

UI_fun(ui_display)::m_float(t_object *z, t_float f)
{
    //TEMP
    
    t_atom argv;
    argv.a_type = A_FLOAT;
    argv.a_w.w_float = f;
    int argc = 1;
    
    ceammc_gui::object<ui_display>::m_anything(z, &s_float, argc, &argv);
}

UI_fun(ui_display)::m_bang(t_object *z, t_symbol *s, int argc, t_atom *argv)
{
    ceammc_gui::object<ui_display>::m_anything(z, &s_list, argc, argv);
}

void display_clock(t_object *z)
{
    ui_display *zx = (ui_display*)z;
    
    zx->bang = false;
    
    ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
}

UI_fun(ui_display)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
{
    ui_display *zx = (ui_display*)z;
    zx->t_e = etext_layout_create();
    
    zx->t_ef = efont_create(gensym("Helvetica"), gensym(""), gensym("normal"), 12);
    
    zx->t_c = clock_new(zx, (t_method)display_clock);
    
    
    
}

UI_fun(ui_display)::free_ext(t_object *z)
{
    ui_display *zx = (ui_display*)z;
    efont_destroy(zx->t_ef);
    
}

UI_fun(ui_display)::init_ext(t_eclass *z)
{
    CLASS_ATTR_DEFAULT (z, "size", 0, "100. 15.");
    
}

extern "C" void setup_ui0x2edisplay()
{
    ceammc_gui::object<ui_display> class1;
    class1.setup("ui.display");
}