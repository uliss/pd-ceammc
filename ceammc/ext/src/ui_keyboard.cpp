//
//  ui_keyboard.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 17/12/16.
//
//

#include "lib/ceammc_gui.h"

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

struct ui_keyboard : public ceammc_gui::base_pd_object
{
    t_ebox x_gui;
    
    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;
    
    t_outlet *out1;
    t_atom out_list[2];
    
    int keys;
    int shift;
    
    int _pitch;
    int _vel;
    int _pitch_prev;
};

namespace ceammc_gui {
    
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
    
    bool mouse_in_rect(int x, int y, kRect kr)
    {
        return (x>kr.x) &&
        x<(kr.x+kr.w) &&
        y>(kr.y) &&
        y<(kr.y+kr.h);
        
    }
    
    
    
    UI_fun(ui_keyboard)::wx_paint(t_object *z, t_object *view)
    {
        //UI_Prop
        
        t_symbol *bgl = gensym("background_layer");
        //float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        if(g)
        
        {
            ui_keyboard *zx = (ui_keyboard*)z;
        
            
            if (zx->keys < 3) zx->keys = 3;                         // rapper's piano meme
            if (zx->keys > 127) zx->keys = 127;
            
            float kWidth = ceil(rect.width/zx->keys);
            
            t_symbol *color_wh_f = gensym("#F0F0F0");
            t_symbol *color_wh_b = gensym("#C0C0C0");
            t_symbol *color_bl_f = gensym("#505050");
            t_symbol *color_bl_b = gensym("#C0C0C0");
            t_symbol *color_hv_b = gensym("#00C0FF");
            t_symbol *color_md_f = gensym("#00C0FF");
            
            
            // two pass draw
            for (int i = 0; i<zx->keys; i++)
            {
                kRect k = get_key_r(i, kWidth, rect.height);
                
                if (!k.is_black)
                {
                    char keyname[7];
                    sprintf(keyname, "KEY%d", i);
                    
                    //bool hover = ( i == int(UI_Pf("_pitch")) );
                    bool hover = ( i == int(zx->_pitch));
                    bool click = zx->mouse_dn; //( UI_Pf("_mouse_dn")==1 );
                    
                    egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                    egraphics_set_color_hex(g, (hover) ? (click ? color_md_f : color_wh_f) : color_wh_f);
                    egraphics_fill(g);
                    
                    egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                    egraphics_set_color_hex(g, (hover) ? (click ? color_hv_b : color_hv_b) : color_wh_b);
                    egraphics_stroke(g);
                    
                }
            }
            
            for (int i = 0; i<zx->keys; i++)
            {
                kRect k = get_key_r(i, kWidth, rect.height);
                
                if (k.is_black)
                {
                    char keyname[7];
                    sprintf(keyname, "KEY%d", i);
                    
                    bool hover = ( i == int(zx->_pitch));
                    bool click = zx->mouse_dn; //( UI_Pf("_mouse_dn")==1 );
                    
                    egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                    egraphics_set_color_hex(g, (hover) ? (click ? color_md_f : color_bl_f) : color_bl_f);
                    egraphics_fill(g);
                    
                    egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                    egraphics_set_color_hex(g, (hover) ? (click ? color_hv_b : color_hv_b) : color_bl_b);
                    egraphics_stroke(g);

                }
            }
            
        }
        
        ebox_end_layer((t_ebox*)z, bgl);
        
        ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
        
    }
    
#pragma mark -
    
    UI_fun(ui_keyboard)::wx_mousemove_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
        //UI_Prop
        
        ui_keyboard *zx = (ui_keyboard*)z;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        //zx->keys = UI_Pf("keys");
        float kWidth = ceil(rect.width/zx->keys);
        
        
        for (int i = 0; i<zx->keys; i++)
        {
            kRect k = get_key_r(i, kWidth, rect.height);
            
            if (!k.is_black)
            {
                
                //TEMP
                //if (mouse_in_rect(UI_Pf("_mouse_x"), UI_Pf("_mouse_y"), k))
                if (mouse_in_rect(zx->mouse_x,zx->mouse_y, k))
                {
                    zx->_pitch = i;
                }
                
            }
        }
        
        for (int i = 0; i<zx->keys; i++)
        {
            kRect k = get_key_r(i, kWidth, rect.height);
            
            if (k.is_black)
            {
                //if (mouse_in_rect(UI_Pf("_mouse_x"), UI_Pf("_mouse_y"), k))
                if (mouse_in_rect(zx->mouse_x,zx->mouse_y, k))
                {
                    zx->_pitch = i;
                }
            }
        }
        
    }
    
    UI_fun(ui_keyboard)::wx_mousedown_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
        //UI_Prop
        
        ui_keyboard *zx = (ui_keyboard*)z;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        zx->_vel= int(pt.y/rect.height*127.);
        
        atom_setfloat(&((ui_keyboard*)z)->out_list[0], (float)zx->_pitch);
        atom_setfloat(&((ui_keyboard*)z)->out_list[1], (float)zx->_vel);
        
        outlet_list( ((ui_keyboard*)z)->out1, &s_list, 2, ((ui_keyboard*)z)->out_list );
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
    }
    
    UI_fun(ui_keyboard)::wx_mouseup_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
        //UI_Prop
        
        ui_keyboard *zx = (ui_keyboard*)z;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        zx->_vel=0;
        
        atom_setfloat(&((ui_keyboard*)z)->out_list[0], (float)zx->_pitch);
        atom_setfloat(&((ui_keyboard*)z)->out_list[1], (float)zx->_vel);
        
        outlet_list( ((ui_keyboard*)z)->out1, &s_list, 2, ((ui_keyboard*)z)->out_list );
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
    }
    
    UI_fun(ui_keyboard)::wx_mousedrag_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {

        ui_keyboard *zx = (ui_keyboard*)z;
        
        ceammc_gui::object<ui_keyboard>::wx_mousemove_ext(z,view,pt,modifiers);
        
        if (zx->_pitch_prev != zx->_pitch )
        {

            zx->_pitch_prev = zx->_pitch;
            
            atom_setfloat(&((ui_keyboard*)z)->out_list[0], (float)zx->_pitch);
            atom_setfloat(&((ui_keyboard*)z)->out_list[1], (float)zx->_vel);
            
            outlet_list( ((ui_keyboard*)z)->out1, &s_list, 2, ((ui_keyboard*)z)->out_list );
            
        }
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
    }
    
    UI_fun(ui_keyboard)::wx_mouseleave_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ui_keyboard *zx = (ui_keyboard*)z;
        zx->_pitch = -1;
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
    }
    
    
#pragma mark -
    
    UI_fun(ui_keyboard)::init_ext(t_eclass *z)
    {
        CLASS_ATTR_DEFAULT (z, "size", 0, "450. 45.");
        
        CLASS_ATTR_INT(z, "shift", 0, ui_keyboard, shift);
        CLASS_ATTR_DEFAULT(z, "shift", 0, "36");
        CLASS_ATTR_LABEL(z, "shift", 0, "shift");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift", 0, "36");
        
        CLASS_ATTR_INT(z, "keys", 0, ui_keyboard, keys);
        CLASS_ATTR_DEFAULT(z, "keys", 0, "61");
        CLASS_ATTR_LABEL(z, "keys", 0, "keys");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "keys", 0, "61");
        
    }
    
    UI_fun(ui_keyboard)::new_ext(t_object *x, t_symbol *s, int argcl, t_atom *argv)
    {
        ((ui_keyboard*)x)->out1 = outlet_new(x, &s_bang);
    }
    
    
//    UI_fun(ui_keyboard)::ui_properties_init_ext(ceammc_gui::properties *def_p)
//    {
//        
//        def_p->ui_property_set("keys", 61.);
//        
//        def_p->ui_property_set("_pitch", 0);
//        
//        
//        
//    }
    
}

extern "C" void setup_ui0x2ekeyboard()
{
    ceammc_gui::object<ui_keyboard> class1;
    class1.setup("ui.keyboard");
}