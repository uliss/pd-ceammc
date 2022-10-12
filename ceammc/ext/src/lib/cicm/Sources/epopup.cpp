/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "epopup.h"

t_epopup* epopupmenu_create(t_eobj* x, t_symbol* name)
{
    auto popup = (t_epopup*)malloc(sizeof(t_epopup));
    if (popup) {
        popup->c_send = x->o_id;
        popup->c_name = name;
        sys_vgui("destroy .eboxpopup%s\n", popup->c_name->s_name);
        sys_vgui("menu .eboxpopup%s -tearoff 0\n", popup->c_name->s_name);
    }
    return popup;
}

void epopupmenu_setfont(t_epopup* popup, t_efont* font)
{
    sys_vgui(".eboxpopup%s configure -font {%s %d %s italic}\n",
        popup->c_name->s_name, font[0].c_family->s_name,
        (int)font[0].c_size, font[0].c_weight->s_name, font[0].c_slant->s_name);
}

void epopupmenu_additem(t_epopup* popup, int itemid, const char* text, bool enabled, const t_pt& pos)
{
    sys_vgui(".eboxpopup%s add command ", popup->c_name->s_name);
    sys_vgui("-command {pdsend {%s popup %s %d %i %i}} ", popup->c_send->s_name, popup->c_name->s_name, itemid, (int)pos.x, (int)pos.y);
    sys_vgui("-label [_ {%s} ] ", text);
    if (enabled)
        sys_vgui("-state active\n");
    else
        sys_vgui("-state disable\n");
}

void epopupmenu_addseparator(t_epopup* popup)
{
    sys_vgui(".eboxpopup%s add separator\n", popup->c_name->s_name);
}

void epopupmenu_popup(t_epopup* popup, t_pt pos)
{
    sys_vgui(".eboxpopup%s post %i %i\n", popup->c_name->s_name, (int)pos.x, (int)pos.y);
}
