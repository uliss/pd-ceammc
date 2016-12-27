//
//  ui_display.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 21/12/16.
//
//

#include <stdio.h>

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include "lib/ceammc_gui.h"

#include <iostream>
#include <string>

using namespace ceammc;

struct ui_display : public ceammc_gui::base_pd_object {
    t_ebox x_gui;
    
    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;
    
    int argc;
    t_atom* argv;
    
    t_etext* txt_type;
    t_etext* txt_val;
    t_efont* txt_font;
    
    t_clock* t_c;
    
    bool bang;
    
    std::string * s_value;
    std::string * s_type;
    AtomList *list;
    
    int show_type;
    int show_bang;
    
};

namespace ceammc_gui {
    
    //TEMP
    template <typename T>
    std::string to_str(const T& t)
    {
        std::ostringstream os;
        os << t;
        return os.str();
    }
    
    static inline t_symbol *msg_color(std::string s_type)
    {
        std::string str1 = "#909090";
        
        if (s_type == "list") str1 = "#00A0C0";
        if (s_type == "float") str1 = "#C000A0";
        
        return gensym(str1.c_str());
    }
    
    UI_fun(ui_display)::wx_paint(t_object* z, t_object* view)
    {
        //UI_Prop
        
        t_symbol* bgl = gensym("background_layer");
        //float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox*)z, &rect);
        
        t_elayer* g = ebox_start_layer((t_ebox*)z, bgl, rect.width, rect.height);
        
        ui_display* zx = (ui_display*)z;
        
        if (g) {
            
            
            if (zx->show_type)
            {
                egraphics_set_color_hex(g, msg_color((*zx->s_type)));
                egraphics_rectangle(g, 0, 0, 45, rect.height);
                egraphics_fill(g);
                
                
                egraphics_set_color_hex(g, gensym(zx->bang ? "#00C0FF" : "#E0E0E0"));
                egraphics_rectangle(g, 45, 0, rect.width, rect.height);
                egraphics_fill(g);
                
                etext_layout_set(zx->txt_type, (*zx->s_type).c_str(), zx->txt_font, 3, rect.height /1 , 45, rect.height, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
                etext_layout_set(zx->txt_val, (*zx->s_value).c_str(), zx->txt_font, 48, rect.height /1 , rect.width-50, rect.height, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
                etext_layout_draw(zx->txt_type, g);
                etext_layout_draw(zx->txt_val, g);
            }
            else
            {
                
                egraphics_set_color_hex(g, gensym(zx->bang ? "#00C0FF" : "#E0E0E0"));
                egraphics_rectangle(g, 0, 0, rect.width, rect.height);
                egraphics_fill(g);
                
                etext_layout_set(zx->txt_val, (*zx->s_value).c_str(), zx->txt_font, 3, rect.height /1 , rect.width-5, rect.height, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
                
                etext_layout_draw(zx->txt_val, g);
            }
            
            
            
        }
        
        ebox_end_layer((t_ebox*)z, bgl);
        ebox_paint_layer((t_ebox*)z, bgl, 0., 0.);
    }
    
    UI_fun(ui_display)::m_anything(t_object* z, t_symbol* s, int argc, t_atom* argv)

    {   
        ui_display* zx = (ui_display*)z;

        (*zx->s_type) = s->s_name;
        (*zx->s_value) = to_string(AtomList(argc, argv));
        
        if (zx->show_bang)
        {
            zx->bang = true;
            clock_delay(zx->t_c, 100);
        }

        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
    }
    
    UI_fun(ui_display)::m_list(t_object* z, t_symbol* s, int argc, t_atom* argv)
    {
        ceammc_gui::object<ui_display>::m_anything(z, &s_list, argc, argv);
    }
    
    UI_fun(ui_display)::m_float(t_object* z, t_float f)
    {
        Atom value1 = Atom(f);
        AtomList list1 = AtomList();
        list1.append(value1);
        ceammc_gui::object<ui_display>::m_anything(z, &s_float, 1, list1.toPdData());
    }
    
    UI_fun(ui_display)::m_bang(t_object* z, t_symbol* s, int argc, t_atom* argv)
    {

        ceammc_gui::object<ui_display>::m_anything(z, &s_bang, 0, 0);
    }
    
    void display_clock(t_object* z)
    {
        ui_display* zx = (ui_display*)z;
        zx->bang = false;
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
    }
    

    UI_fun(ui_display)::wx_oksize(t_object *z, t_rect *newrect)
    {
        newrect->height = (newrect->height>15)? newrect->height : 15;
        
    }
    

    UI_fun(ui_display)::new_ext(t_object* z, t_symbol* s, int argcl, t_atom* argv)
    {
        ui_display* zx = (ui_display*)z;
        
        zx->s_value = new std::string;
        zx->s_type = new std::string;
        
        zx->txt_val = etext_layout_create();
        zx->txt_type = etext_layout_create();
        
        zx->txt_font = efont_create(gensym("Helvetica"), gensym(""), gensym("normal"), 12);
        
        zx->t_c = clock_new(zx, (t_method)display_clock);
        
        zx->show_bang = 1;
        zx->show_type = 0;
        
    }
    
    UI_fun(ui_display)::free_ext(t_object* z)
    {
        ui_display* zx = (ui_display*)z;
        delete zx->s_value;
        delete zx->s_type;
        
        efont_destroy(zx->txt_font);
        
        etext_layout_destroy(zx->txt_type);
        etext_layout_destroy(zx->txt_val);
    }
    
    UI_fun(ui_display)::init_ext(t_eclass* z)
    {
        CLASS_ATTR_DEFAULT(z, "size", 0, "120. 15.");
        
        CLASS_ATTR_INT(z, "display_events", 0, ui_display, show_bang);
        CLASS_ATTR_DEFAULT(z, "display_events", 0, "1");
        CLASS_ATTR_LABEL(z, "display_events", 0, "Display events");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "display_events", 0, "1");
        
        CLASS_ATTR_INT(z, "display_type", 0, ui_display, show_type);
        CLASS_ATTR_DEFAULT(z, "display_type", 0, "0");
        CLASS_ATTR_LABEL(z, "display_type", 0, "Display type");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "display_type", 0, "0");
        
    }
}

extern "C" void setup_ui0x2edisplay()
{
    ceammc_gui::object<ui_display> class1;
    class1.setup("ui.display");
}
