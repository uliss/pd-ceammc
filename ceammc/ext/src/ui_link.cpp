//
//  ui_link.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 22/12/16.
//
//

#include <stdio.h>

#include "lib/ceammc_gui.h"

#include "m_imp.h"  /* FIXME need access to c_externdir... */
//#include "g_canvas.h"

//#include "s_path.c"

#include <string>
#include <iostream>

struct ui_link : ceammc_gui::base_pd_object
{
    t_ebox x_gui;
    
    t_etext *t_e;
    t_efont *t_ef;
    
    t_symbol *link;
    t_symbol *title;
    
    t_canvas *x_canvas;
    t_symbol *x_dirsym;
    
    // std::string display;
    
};

//TEMP
template <typename T> std::string to_str(const T& t) {
    std::ostringstream os;
    os<<t;
    return os.str();
}

//#pragma mark -
//
////TEMPORARY
////IEM / l2ork s_path.c
//
//
//static void sys_path_replace(
//                             char const * const original,
//                             char * returned,
//                             char const * const pattern,
//                             char const * const replacement
//                             ) {
//    size_t const replen = strlen(replacement);
//    size_t const patlen = strlen(pattern);
//    
//    size_t patcnt = 0;
//    const char * oriptr;
//    const char * patloc;
//    
//    // find how many times the pattern occurs in the original string
//    for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
//    {
//        patcnt++;
//    }
//    
//    {
//        // allocate memory for the new string
//        if (returned != NULL)
//        {
//            // copy the original string,
//            // replacing all the instances of the pattern
//            char *retptr = returned;
//            for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
//            {
//                size_t const skplen = patloc - oriptr;
//                // copy the section until the occurence of the pattern
//                strncpy(retptr, oriptr, skplen);
//                retptr += skplen;
//                // copy the replacement
//                strncpy(retptr, replacement, replen);
//                retptr += replen;
//            }
//            // copy the rest of the string.
//            strcpy(retptr, oriptr);
//        }
//    }
//}
//
///* expand env vars and ~ at the beginning of a path and make a copy to return */
//static void sys_expandpath(const char *from, char *to)
//{
//    if ((strlen(from) == 1 && from[0] == '~') || (strncmp(from,"~/", 2) == 0))
//    {
//#ifdef MSW
//        const char *home = getenv("USERPROFILE");
//#else
//        const char *home = getenv("HOME");
//#endif
//        if(home)
//        {
//            strncpy(to, home, FILENAME_MAX - 1);
//            strncat(to, from + 1, FILENAME_MAX - strlen(from) - 2);
//        }
//    }
//    else
//        strncpy(to, from, FILENAME_MAX - 1);
//#ifdef MSW
//    char buf[FILENAME_MAX];
//    ExpandEnvironmentStrings(to, buf, FILENAME_MAX - 2);
//    strncpy(to, buf, FILENAME_MAX - 1);
//#endif
//}
//
//
///* used for expanding paths for various objects */
//void sys_expandpathelems(const char *name, char *result)
//{
//    //check for expandable elements in path (e.g. @pd_extra, ~/) and replace
//    //fprintf(stderr,"sys_expandpathelems name=<%s>\n", name);
//    char interim[FILENAME_MAX];
////    if (strstr(name, "@pd_extra") != NULL)
////    {
////        t_namelist *path = pd_extrapath;
////        while (path->nl_next)
////            path = path->nl_next;
////        sys_path_replace(name, interim, "@pd_extra", path->nl_string);
////        //fprintf(stderr,"path->nl_string=<%s>\n", path->nl_string);
////    }
////    else if (strstr(name, "@pd_help") != NULL)
////    {
////        t_namelist *path = sys_helppath;
////        sys_path_replace(name, interim, "@pd_help", path->nl_string);
////        //fprintf(stderr,"path->nl_string=<%s>\n", path->nl_string);
////    }
////    else
//    {
//        strcpy(interim, name);
//    }
//    //fprintf(stderr,"sys_expandpathelems interim=<%s>\n", interim);
//    sys_expandpath(interim, result);
//    //fprintf(stderr,"sys_expandpathelems result=<%s>\n", result);
//}
//
///* test if path is absolute or relative, based on leading /, env vars, ~, etc */
//int sys_isabsolutepath(const char *dir)
//{
//    if (dir[0] == '/' || dir[0] == '~'
//#ifdef MSW
//        || dir[0] == '%' || (dir[1] == ':' && dir[2] == '/')
//#endif
//        )
//    {
//        return 1;
//    }
//    else
//    {
//        return 0;            
//    }
//}


//#pragma mark -


namespace ceammc_gui {
    
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
            //test
            
//            egraphics_set_color_hex(g, gensym("#E0E0E0"));
//            egraphics_rectangle(g, 0, 0, rect.width, rect.height);
//            egraphics_fill(g);
            
            
            egraphics_set_color_hex(g, gensym("#00A0C0"));
            etext_layout_set(zx->t_e, zx->title->s_name, zx->t_ef, 3, rect.height, rect.width, rect.height, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
            
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
        
//        char final_name[FILENAME_MAX];
//        sys_expandpathelems(x->x_ulink->s_name, final_name);
        
        
        
        sys_vgui("pddplink_open {%s} {%s}\n",
                 zx->link->s_name, zx->x_dirsym );
        
        
        
    }
    
    UI_fun(ui_link)::wx_mouseenter(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ui_link *zx = (ui_link*)z;
        zx->t_ef->c_weight = gensym("bold");
        
        ceammc_gui::object<base_pd_object>::ws_redraw(z);
    }
    
    UI_fun(ui_link)::wx_mouseleave(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ui_link *zx = (ui_link*)z;
        zx->t_ef->c_weight = gensym("normal");
        
        ceammc_gui::object<base_pd_object>::ws_redraw(z);
    
    }
    
    static void link_getdrawparams(t_object *z, t_object *view, t_edrawparams *params)
    {
        params->d_borderthickness = 1;
        
        params->d_bordercolor.red = 1;
        params->d_bordercolor.green = 1;
        params->d_bordercolor.blue = 1;
        
        params->d_cornersize = 0;
    }
    
    UI_fun(ui_link)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
    {
        ui_link *zx = (ui_link*)z;
        
        zx->t_e = etext_layout_create();
        
        zx->t_ef = efont_create(gensym("Helvetica"), gensym(""), gensym("normal"), 12);
        
        zx->x_canvas = canvas_getcurrent();
        zx->x_dirsym = canvas_getdir(zx->x_canvas);
        
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
        
        eclass_addmethod(z, (method) link_getdrawparams, "getdrawparams", A_NULL, 0);
    }
    
}

extern "C" void setup_ui0x2elink()
{
    ceammc_gui::object<ui_link> class1;
    class1.setup_noin("ui.link");
}