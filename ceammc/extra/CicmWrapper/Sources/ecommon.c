/*
 * CicmWrapper - Pure Data Enhanced
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "ecommon.h"
#include "egraphics.h"

t_symbol* s_null;
t_symbol* s_atom;
t_symbol* s_obj;
t_symbol* s_attr_modified;
t_symbol* s_eboxbd;
t_symbol* s_eboxio;
t_symbol* s_size;
t_symbol* s_pinned;
t_symbol* s_iscicm;
t_symbol* s_int;
t_symbol* s_long;
t_symbol* s_double;
t_symbol* s_attr_size;
t_symbol* s_color_black_hex;

void epd_init(void)
{
    t_symbol* epd_symbol = gensym("epd1572");
    s_null          = gensym("(null)");
    s_atom          = gensym("atom");
    s_obj           = gensym("obj");
    s_attr_modified = gensym("attr_modified");
    s_eboxbd        = gensym("eboxbd");
    s_eboxio        = gensym("eboxio");
    s_size          = gensym("size");
    s_int           = gensym("int");
    s_long          = gensym("long");
    s_double        = gensym("double");
    s_pinned        = gensym("pinned");
    s_iscicm        = gensym("iscicm");
    s_attr_size     = gensym("@size");
    s_color_black_hex = gensym("#000000");
    if(!epd_symbol->s_thing)
    {
        // PATCHER MOUSE MOTION //
        sys_vgui("proc eobj_canvas_motion {patcher val} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_motion $patcher [expr $x - $rx] [expr $y - $ry] $val\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE DOWN //
        sys_vgui("proc eobj_canvas_down {patcher val} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_mouse $patcher [expr $x - $rx] [expr $y - $ry] 0 $val\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE UP //
        sys_vgui("proc eobj_canvas_up {patcher} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_mouseup $patcher [expr $x - $rx] [expr $y - $ry] 0\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE RIGHT //
        sys_vgui("proc eobj_canvas_right {patcher} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_rightclick $patcher [expr $x - $rx] [expr $y - $ry] 0\n");
        sys_gui("}\n");
        
        // OBJECT SAVE FILE //
        sys_gui("proc eobj_saveas {name initialfile initialdir} {\n");
        sys_gui("if { ! [file isdirectory $initialdir]} {set initialdir $::env(HOME)}\n");
        sys_gui("set filename [tk_getSaveFile -initialfile $initialfile -initialdir $initialdir -defaultextension .pd -filetypes $::filetypes]\n");
        sys_gui("if {$filename eq \"\"} return;\n");
        
        sys_gui("set extension [file extension $filename]\n");
        sys_gui("set oldfilename $filename\n");
        
        sys_gui("if {$filename ne $oldfilename && [file exists $filename]} {\n");
        sys_gui("set answer [tk_messageBox -type okcancel -icon question -default cancel-message [_ \"$filename\" already exists. Do you want to replace it?]]\n");
        sys_gui("if {$answer eq \"cancel\"} return;\n");
        sys_gui("}\n");
        sys_gui("set dirname [file dirname $filename]\n");
        sys_gui("set basename [file tail $filename]\n");
        sys_gui("pdsend \"$name eobjwriteto [enquote_path $dirname/$basename]\"\n");
        sys_gui("set ::filenewdir $dirname\n");
        sys_gui("::pd_guiprefs::update_recentfiles $filename\n");
        sys_gui("}\n");
        
        // OBJECT OPEN FILE //
        sys_gui("proc eobj_openfrom {name} {\n");
        sys_gui("if { ! [file isdirectory $::filenewdir]} {\n");
        sys_gui("set ::filenewdir [file normalize $::env(HOME)]\n");
        sys_gui("}\n");
        sys_gui("set files [tk_getOpenFile -multiple true -initialdir $::fileopendir]\n");
        sys_gui("pdsend \"$name eobjreadfrom [enquote_path $files]\"\n");
        sys_gui("}\n");
        
        // RGBA TO HEX //
        sys_gui("proc eobj_rgba_to_hex {red green blue alpha} { \n");
        sys_gui("set nR [expr int( $red * 65025 )]\n");
        sys_gui("set nG [expr int( $green * 65025 )]\n");
        sys_gui("set nB [expr int( $blue * 65025 )]\n");
        sys_gui("set col [format {%4.4x} $nR]\n");
        sys_gui("append col [format {%4.4x} $nG]\n");
        sys_gui("append col [format {%4.4x} $nB]\n");
        sys_gui("return #$col\n");
        sys_gui("}\n");
        
        // DRAW CUBIC //
        /*
        sys_gui("proc eobj_bezier_fill{id points } {\n");
        sys_gui("set size [expr {[llength $points] / 2}]\n");
        sys_gui("for {set i 0} {$i < $size} {set i [expr {$i + 3}]} {\n");
        sys_gui("for {set j 0} {$j < 20} {incr j} {\n");
        
        sys_gui(" if {$filled} {$id create line $x1 $y1}");
        sys_gui(" else {$id create polygon $x1 $y1}");
        sys_gui("}\n");
        */
        
        epd_symbol->s_thing = (t_class **)1;
    }
}

void object_method(void* x, t_symbol* s, void* z, t_typ_method method, long number, void* other)
{
    t_ret_method nrmethod = (t_ret_method)getfn((t_pd *)x, s);
    nrmethod(x, s, z, method, number, other);
}

t_binbuf* binbuf_via_atoms(int ac, t_atom *av)
{
    t_binbuf* dico = binbuf_new();
    if(dico)
    {
        binbuf_add(dico, ac, av);
    }
    return dico;
}

static t_symbol* format_symbol(t_symbol* s)
{
    int i, j, lenght = (int)strlen(s->s_name);
    char buffer[MAXPDSTRING];
    buffer[0] = '\"';
    for(i = 0, j = 1; i < lenght && j < MAXPDSTRING - 2; i++, j++)
    {
        if(s->s_name[i] == '"' || s->s_name[i] == '\\')
        {
            buffer[j++] = '\\';
        }
        buffer[j] = s->s_name[i];
    }
    buffer[j++] = '\"';
    buffer[j] = '\0';
    return gensym(buffer);
}

static t_atom* format_atoms(int ac, t_atom* av)
{
    int i;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYMBOL)
        {
            atom_setsym(av+i, format_symbol(atom_getsymbol(av+i)));
        }
        else
        {
            av[i] = av[i];
        }
    }
    return av;
}

static long unformat_symbol(char* text, char* buffer, long size)
{
    int i = 0, j = 0, lenght = (int)strlen(text);
    int end = text[lenght - 1] == '"' || text[lenght - 1] == '\'';
    for(; i < lenght - end && j < size - 1; i++)
    {
        if(text[i] != '\\')
        {
            buffer[j++] = text[i];
        }
    }
    buffer[j] = '\0';
    return !end;
}

static long unformat_atoms(int ac, t_atom* av)
{
    int i, lenght, newize = 0;
    char str = 0;
    char temp[256];
    char buffer[MAXPDSTRING];
    t_symbol* s;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYMBOL)
        {
            s = atom_getsymbol(av+i);
            if(strcmp(s->s_name, "[") && strcmp(s->s_name, "]"))
            {
                if(!str)
                {
                    if(s->s_name[0] == '"' || s->s_name[0] == '\'')
                    {
                        str = (char)unformat_symbol(s->s_name+1, buffer, MAXPDSTRING);
                    }
                    else
                    {
                        unformat_symbol(s->s_name, buffer, MAXPDSTRING);
                    }
                }
                else
                {
                    lenght = (int)strlen(buffer);
                    strncat(buffer, " ", 1);
                    str = (char)unformat_symbol(s->s_name, buffer+lenght+1, MAXPDSTRING-lenght-1);
                }
                if(!str)
                {
                    atom_setsym(av+newize, gensym(buffer));
                    //sprintf(buffer, "");
                    //buffer[0] = '\0'; //-> is equal to sprintf(buffer, "") but only change the first caractere
                    memset(buffer, '\0', MAXPDSTRING*sizeof(char)); //-> clean all the buffer
                    newize++;
                }
            }
        }
        else if(str)
        {
            sprintf(temp, " %f", atom_getfloat(av+i));
            lenght = (int)strlen(temp);
            while(temp[lenght - 1] == '0')
            {
                temp[lenght - 1] = '\0';
                lenght--;
            }
            if(temp[lenght - 1] == '.')
            {
                temp[lenght - 1] = '\0';
                lenght--;
            }
            strncat(buffer, temp, lenght);
        }
        else
        {
            av[newize++] = av[i];
        }
    }

    return newize;
}

t_pd_err binbuf_append_attribute(t_binbuf *d, t_symbol *key, int argc, t_atom *argv)
{
    if(d && key && argc && argv)
    {
        format_atoms(argc, argv);
        binbuf_addv(d, "s", key);
        binbuf_add(d, argc, argv);
        return 0;
    }
    return -1;
}

int atoms_get_attributes_offset(int argc, t_atom* argv)
{
    int i;
    for(i = 0; i < argc; i++)
    {
        if(atom_gettype(argv+i) == A_SYMBOL && atom_getsymbol(argv+i)->s_name[0] == '@')
        {
            break;
        }
    }
    return (int)pd_clip_minmax(i, 0, argc);
}

int binbuf_get_attributes_offset(t_binbuf *d)
{
    return atoms_get_attributes_offset(binbuf_getnatom(d), binbuf_getvec(d));
}

int atoms_get_nattributes(int argc, t_atom* argv)
{
    int i, j;
    for(i = 0, j = 0; i < argc; i++)
    {
        if(atom_gettype(argv+i) == A_SYMBOL && atom_getsymbol(argv+i)->s_name[0] == '@')
        {
            j++;
        }
    }
    return j;
}

int binbuf_get_nattributes(t_binbuf *d)
{
    return atoms_get_nattributes(binbuf_getnatom(d), binbuf_getvec(d));
}

int atoms_get_keys(int ac, t_atom* av, t_symbol*** keys)
{
    int i, j;
    int size = atoms_get_nattributes(ac, av);
    if(size)
    {
        keys[0] = (t_symbol**)malloc((size_t)size * sizeof(t_symbol *));
        if(keys[0])
        {
            for(i = 0, j = 0; i < ac; i++)
            {
                if(atom_gettype(av+i) == A_SYMBOL && atom_getsymbol(av+i)->s_name[0] == '@')
                {
                    keys[0][j] = atom_getsymbol(av+i);
                    j++;
                }
            }
            return size;
        }
    }
    return 0;
}

int binbuf_get_keys(t_binbuf *d, t_symbol*** keys)
{
    return atoms_get_keys(binbuf_getnatom(d), binbuf_getvec(d), keys);
}


t_pd_err atoms_has_attribute(int argc, t_atom* argv, t_symbol *key)
{
    int i;
    if(argc && argv)
    {
        for(i = 0; i < argc; i++)
        {
            if(atom_gettype(argv+i) == A_SYMBOL && atom_getsymbol(argv+i) == key)
            {
                return 1;
            }
        }
    }
    return 0;
}

t_pd_err binbuf_has_attribute(t_binbuf *d, t_symbol *key)
{
    if(d)
        return atoms_has_attribute(binbuf_getnatom(d), binbuf_getvec(d), key);
    else
        return -1;
}

int atoms_get_attribute_index(int argc, t_atom *argv, t_symbol *key)
{
    int i;
    if(argc && argv)
    {
        for(i = 0; i < argc; i++)
        {
            if(atom_gettype(argv+i) == A_SYMBOL && atom_getsymbol(argv+i) == key)
            {
                return i;
                break;
            }
        }
    }
    return -1;
}

int binbuf_get_attribute_index(t_binbuf *d, t_symbol *key)
{
    return atoms_get_attribute_index(binbuf_getnatom(d), binbuf_getvec(d), key);
}

t_pd_err atoms_get_attribute(int ac, t_atom* av, t_symbol *key, int *argc, t_atom **argv)
{
    t_atom* temp;
    long i = 0, index  = 0, newsize = 0;
    argc[0]     = 0;
    argv[0]     = NULL;

    index = atoms_get_attribute_index(ac, av, key) + 1;
    if(index > 0)
    {
        i = index;
        while(i < ac && atom_getsymbol(av+i)->s_name[0] != '@')
        {
            i++;
            argc[0]++;
        }
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }

    if(argc[0])
    {
        argv[0] = (t_atom *)malloc((size_t)argc[0] * sizeof(t_atom));
        if(argv[0])
        {
            memcpy(argv[0], av+index, (size_t)argc[0] * sizeof(t_atom));
            newsize = unformat_atoms(argc[0], argv[0]);
            if(newsize)
            {
                temp = (t_atom *)realloc(argv[0], (size_t)newsize * sizeof(t_atom));
                if(temp)
                {
                    argv[0] = temp;
                    argc[0] = (int)newsize;
                    return 0;
                }
                else
                {
                    free(argv[0]);
                    argv[0] = NULL;
                    argc[0] = 0;
                }
            }
            else
            {
                free(argv[0]);
            }
        }
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }

    return 0;
}

t_pd_err binbuf_get_attribute(t_binbuf *d, t_symbol *key, int *argc, t_atom **argv)
{
    if(d)
        return atoms_get_attribute(binbuf_getnatom(d), binbuf_getvec(d), key, argc, argv);
    else
        return -1;
}

t_pd_err atoms_get_attribute_int(int ac, t_atom* av, t_symbol *key, int *value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if(!atoms_get_attribute(ac, av, key, &argc, &argv))
    {
        if(argc && argv)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                value[0] = (int)atom_getfloat(argv);
                free(argv);
                return 0;
            }
            free(argv);
        }
        return -1;
    }
    return -1;
}

t_pd_err binbuf_get_attribute_int(t_binbuf *d, t_symbol *key, int *value)
{
    if(d)
        return atoms_get_attribute_int(binbuf_getnatom(d), binbuf_getvec(d), key, value);
    else
        return -1;
}

t_pd_err atoms_get_attribute_long(int ac, t_atom* av, t_symbol *key, long *value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if(!atoms_get_attribute(ac, av, key, &argc, &argv))
    {
        if(argc && argv)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                value[0] = atom_getlong(argv);
                free(argv);
                return 0;
            }
            free(argv);
        }
        return -1;
    }
    return -1;
}

t_pd_err binbuf_get_attribute_long(t_binbuf *d, t_symbol *key, long *value)
{
    if(d)
        return atoms_get_attribute_long(binbuf_getnatom(d), binbuf_getvec(d), key, value);
    else
        return -1;
}

t_pd_err atoms_get_attribute_float(int ac, t_atom* av, t_symbol *key, float *value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if(!atoms_get_attribute(ac, av, key, &argc, &argv))
    {
        if(argc && argv)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                value[0] = atom_getfloat(argv);
                free(argv);
                return 0;
            }
            free(argv);
        }
        return -1;
    }
    return -1;
}

t_pd_err binbuf_get_attribute_float(t_binbuf *d, t_symbol *key, float *value)
{
    if(d)
        return atoms_get_attribute_float(binbuf_getnatom(d), binbuf_getvec(d), key, value);
    else
        return -1;
}

void epd_add_folder(const char* name, const char* folder)
{
    /*
    // ADD TO SYSTEM PATH
    sys_gui("foreach pathdir [concat $::sys_searchpath $::sys_staticpath] {\n");
    sys_gui("if {[file isdirectory $pathdir]} {\n");
    sys_gui("set dir [string trimright [file join [file normalize $pathdir] { }]]\n");
    sys_vgui("append dir \"%s\"\n", name);
    sys_gui("if {[file isdirectory $dir] && [file exists $dir]} {\n");
    sys_vgui("append dir \"/%s/\"\n", folder);
    
    sys_gui("if {[file isdirectory $dir] && [file exists $dir]} {::helpbrowser::add_entry ::sys_searchpath dir}\n");
            //lappend ::sys_staticpath $dir\n pdwindow::logpost 0, 2 \"OK\n\"}\n");
    
    sys_gui("}}}\n");
    post("start");
    const char dir[MAXPDSTRING], ext = '\0';
    char dirresult[MAXPDSTRING], *nameresult = NULL;
    if(open_via_path(dir, folder, &ext, dirresult, &nameresult, MAXPDSTRING, 0) != -1)
    {
        post("aki");
    }
    post("end");
    */

	char path[MAXPDSTRING];
	t_namelist* var = sys_searchpath;
	while (var)
	{
		sprintf(path, "%s/%s",var->nl_string, name);
		if(strncmp(var->nl_string, name, strlen(name)) == 0)
		{
			sprintf(path, "%s/%s", var->nl_string, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		else if(access(path, O_RDONLY) != -1)
		{
			sprintf(path, "%s/%s/%s", var->nl_string, name, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		var = var->nl_next;
	}
    var = sys_staticpath;
    while (var)
    {
        sprintf(path, "%s/%s",var->nl_string, name);
		if(strncmp(var->nl_string, name, strlen(name)) == 0)
		{
			sprintf(path, "%s/%s", var->nl_string, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		else if(access(path, O_RDONLY) != -1)
		{
			sprintf(path, "%s/%s/%s", var->nl_string, name, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		var = var->nl_next;
    }

}



