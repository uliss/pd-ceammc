//
//  ui_keyboard1.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 17/12/16.
//
//

#include "ceammc_gui_cicm.h"

struct ui_keyboard1 : cm_gui_base_pd_object
{
    t_ebox x_gui;
    t_outlet *out1;
    
};

struct kRect
{
    int x,y,w,h;
    bool is_black;
};

kRect get_black_key_r(int offset, int kWidth, int kHeight)
{
    kRect ret;
    
    int n_number = offset % 12;
    int n_octave = offset/12;
    
    
    ret.x = (offset + (n_number>4) + 2*n_octave)* kWidth + .5*kWidth;
    ret.y = 0;
    ret.w = kWidth;
    ret.h = kHeight*.6;
    ret.is_black = true;
    
    return ret;
}

kRect get_white_key_r(int offset, int kWidth, int kHeight)
{
    kRect ret;
    
    int n_number = offset % 12;
    int n_octave = offset/12;
    
    ret.x = (offset + (n_number>4) + 2*n_octave)*   kWidth;
    ret.y = 0;
    ret.w = kWidth*2;
    ret.h = kHeight;
    ret.is_black = false;
    
    return ret;
    
}

kRect get_key_r(int number, int kWidth, int kHeight)
{
    int n_number = number % 12;
    bool is_black = ((n_number -  (n_number<5)) % 2);
    
    kRect ret =
    (!is_black)? get_black_key_r( number,  kWidth,  kHeight ) : get_white_key_r( number  ,  kWidth,  kHeight );
    
    return ret;
}

UI_fun(ui_keyboard1)::wx_paint(t_object *z, t_object *view)
{
    UI_Prop
    
    t_symbol *bgl = gensym("background_layer");
    float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    if(g)
        
    {
        
        if ( UI_Pf("keys") < 3 ) UI_Pset("keys",3);             // rapper's piano meme
        if ( UI_Pf("keys") > 127 ) UI_Pset("keys",127);
        
        int numKeys = UI_Pf("keys");
        int kWidth = rect.width/UI_Pf("keys");

        // two pass draw
        for (int i = 0; i<numKeys; i++)
        {
            kRect k = get_key_r(i, kWidth, rect.height);
            
            if (!k.is_black)
            {
                char keyname[7];
                sprintf(keyname, "KEY%d", i);
                
                egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                egraphics_set_color_hex(g, gensym("#E0E0E0"));
                egraphics_fill(g);
                
                egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                egraphics_set_color_hex(g, gensym("#00C0FF"));
                egraphics_stroke(g);

                
            }
        }
        
        for (int i = 0; i<numKeys; i++)
        {
            kRect k = get_key_r(i, kWidth, UI_Pf("height"));
            
            if (k.is_black)
            {
                char keyname[7];
                sprintf(keyname, "KEY%d", i);
                
                egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                egraphics_set_color_hex(g, gensym("#707070"));
                egraphics_fill(g);
                
                egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                egraphics_set_color_hex(g, gensym("#00C0FF"));
                egraphics_stroke(g);
                
            }
        }
        
    }
    
    ebox_end_layer((t_ebox*)z, bgl);

    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
}


UI_fun(ui_keyboard1)::new_ext(t_object *x, t_symbol *s, int argcl, t_atom *argv)
{
    ((ui_keyboard1*)x)->out1 = outlet_new(x, &s_bang);
}


UI_fun(ui_keyboard1)::ui_properties_init_ext(cm_gui_properties *def_p)
{
    
    def_p->ui_property_set("width", 450.);
    def_p->ui_property_set("height", 45.);
    def_p->ui_property_set("keys", 61.);
    
    
    
}

extern "C" void setup_ui0x2ekeyboard1()
{
    cm_gui_object<ui_keyboard1> class1;
    class1.setup("ui.keyboard1");
}