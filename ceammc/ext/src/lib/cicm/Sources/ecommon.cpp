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
#include "cicm_common.tcl.h"
#include "cicm_common_bind.tcl.h"
#include "egraphics.h"

void epd_init(void)
{
    static bool done = false;
    if (done)
        return;

    // split long output
    char buf[1024];
    const size_t LEN = strlen(cicm_common_tcl);
    const size_t BLEN = sizeof(buf)-1;
    const size_t N = LEN / BLEN;
    for(size_t i = 0; i <= N; i++) {
        strncpy(buf, cicm_common_tcl + i * BLEN, BLEN);
        buf[BLEN] = '\0';
        sys_gui(buf);
    }

    sys_gui(cicm_common_bind_tcl);

    sys_vgui("ceammc::ui::bindKeys\n");

    done = true;
}

void object_method(void* x, t_symbol* s, void* z, t_typ_method method, long number, void* other)
{
    auto nrmethod = (t_ret_method)getfn((t_pd*)x, s);
    nrmethod(x, s, z, method, number, other);
}

t_binbuf* binbuf_via_atoms(int ac, t_atom* av)
{
    t_binbuf* dico = binbuf_new();
    if (dico) {
        binbuf_add(dico, ac, av);
    }
    return dico;
}

static t_symbol* format_symbol(t_symbol* s)
{
    int i, j, len = (int)strlen(s->s_name);
    char buf[MAXPDSTRING];
    buf[0] = '\"';
    for (i = 0, j = 1; i < len && j < MAXPDSTRING - 2; i++, j++) {
        if (s->s_name[i] == '"' || s->s_name[i] == '\\') {
            buf[j++] = '\\';
        }
        buf[j] = s->s_name[i];
    }
    buf[j++] = '\"';
    buf[j] = '\0';
    return gensym(buf);
}

static t_atom* format_atoms(int ac, t_atom* av)
{
    for (int i = 0; i < ac; i++) {
        if (atom_gettype(av + i) == A_SYMBOL) {
            atom_setsym(av + i, format_symbol(atom_getsymbol(av + i)));
        }
    }
    return av;
}

static long unformat_symbol(const char* text, char* buffer, long size)
{
    int i = 0, j = 0, len = (int)strlen(text);
    int end = text[len - 1] == '"' || text[len - 1] == '\'';
    for (; i < len - end && j < size - 1; i++) {
        if (text[i] != '\\') {
            buffer[j++] = text[i];
        }
    }
    buffer[j] = '\0';
    return !end;
}

static long unformat_atoms(int ac, t_atom* av)
{
    int len, newize = 0;
    char str = 0;
    char temp[256];
    char buf[MAXPDSTRING];
    t_symbol* s;
    for (int i = 0; i < ac; i++) {
        if (atom_gettype(av + i) == A_SYMBOL) {
            s = atom_getsymbol(av + i);
            if (strcmp(s->s_name, "[") && strcmp(s->s_name, "]")) {
                if (!str) {
                    if (s->s_name[0] == '"' || s->s_name[0] == '\'') {
                        str = (char)unformat_symbol(s->s_name + 1, buf, MAXPDSTRING);
                    } else {
                        unformat_symbol(s->s_name, buf, MAXPDSTRING);
                    }
                } else {
                    len = (int)strlen(buf);
                    strncat(buf, " ", sizeof(buf) - 1);
                    str = (char)unformat_symbol(s->s_name, buf + len + 1, MAXPDSTRING - len - 1);
                }
                if (!str) {
                    atom_setsym(av + newize, gensym(buf));
                    //sprintf(buffer, "");
                    //buffer[0] = '\0'; //-> is equal to sprintf(buffer, "") but only change the first caractere
                    memset(buf, '\0', MAXPDSTRING * sizeof(char)); //-> clean all the buffer
                    newize++;
                }
            }
        } else if (str) {
            sprintf(temp, " %f", atom_getfloat(av + i));
            len = (int)strlen(temp);
            while (temp[len - 1] == '0') {
                temp[len - 1] = '\0';
                len--;
            }
            if (temp[len - 1] == '.') {
                temp[len - 1] = '\0';
                len--;
            }
            strncat(buf, temp, len);
        } else {
            av[newize++] = av[i];
        }
    }

    return newize;
}

t_pd_err binbuf_append_attribute(t_binbuf* d, t_symbol* key, int argc, t_atom* argv)
{
    if (d && key && argc && argv) {
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
    for (i = 0; i < argc; i++) {
        if (atom_gettype(argv + i) == A_SYMBOL && atom_getsymbol(argv + i)->s_name[0] == '@') {
            break;
        }
    }
    return (int)pd_clip_minmax(i, 0, argc);
}

int binbuf_get_attributes_offset(t_binbuf* d)
{
    return atoms_get_attributes_offset(binbuf_getnatom(d), binbuf_getvec(d));
}

int atoms_get_nattributes(int argc, t_atom* argv)
{
    int i, j;
    for (i = 0, j = 0; i < argc; i++) {
        if (atom_gettype(argv + i) == A_SYMBOL && atom_getsymbol(argv + i)->s_name[0] == '@') {
            j++;
        }
    }
    return j;
}

int binbuf_get_nattributes(t_binbuf* d)
{
    return atoms_get_nattributes(binbuf_getnatom(d), binbuf_getvec(d));
}

int atoms_get_keys(int ac, t_atom* av, t_symbol*** keys)
{
    int size = atoms_get_nattributes(ac, av);
    if (size) {
        keys[0] = (t_symbol**)malloc((size_t)size * sizeof(t_symbol*));
        if (keys[0]) {
            for (int i = 0, j = 0; i < ac; i++) {
                if (atom_gettype(av + i) == A_SYMBOL && atom_getsymbol(av + i)->s_name[0] == '@') {
                    keys[0][j] = atom_getsymbol(av + i);
                    j++;
                }
            }
            return size;
        }
    }
    return 0;
}

int binbuf_get_keys(t_binbuf* d, t_symbol*** keys)
{
    return atoms_get_keys(binbuf_getnatom(d), binbuf_getvec(d), keys);
}

t_pd_err atoms_has_attribute(int argc, t_atom* argv, t_symbol* key)
{
    if (argc && argv) {
        for (int i = 0; i < argc; i++) {
            if (atom_gettype(argv + i) == A_SYMBOL && atom_getsymbol(argv + i) == key) {
                return 1;
            }
        }
    }
    return 0;
}

t_pd_err binbuf_has_attribute(t_binbuf* d, t_symbol* key)
{
    if (d)
        return atoms_has_attribute(binbuf_getnatom(d), binbuf_getvec(d), key);
    else
        return -1;
}

int atoms_get_attribute_index(int argc, t_atom* argv, t_symbol* key)
{
    if (argc && argv) {
        for (int i = 0; i < argc; i++) {
            if (atom_gettype(argv + i) == A_SYMBOL && atom_getsymbol(argv + i) == key) {
                return i;
                break;
            }
        }
    }
    return -1;
}

int binbuf_get_attribute_index(t_binbuf* d, t_symbol* key)
{
    return atoms_get_attribute_index(binbuf_getnatom(d), binbuf_getvec(d), key);
}

t_pd_err atoms_get_attribute(int ac, t_atom* av, t_symbol* key, int* argc, t_atom** argv)
{
    t_atom* temp;
    long i = 0, index = 0, newsize = 0;
    argc[0] = 0;
    argv[0] = NULL;

    index = atoms_get_attribute_index(ac, av, key) + 1;
    if (index > 0) {
        i = index;
        while (i < ac && atom_getsymbol(av + i)->s_name[0] != '@') {
            i++;
            argc[0]++;
        }
    } else {
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }

    if (argc[0]) {
        argv[0] = (t_atom*)malloc((size_t)argc[0] * sizeof(t_atom));
        if (argv[0]) {
            memcpy(argv[0], av + index, (size_t)argc[0] * sizeof(t_atom));
            newsize = unformat_atoms(argc[0], argv[0]);
            if (newsize) {
                temp = (t_atom*)realloc(argv[0], (size_t)newsize * sizeof(t_atom));
                if (temp) {
                    argv[0] = temp;
                    argc[0] = (int)newsize;
                    return 0;
                } else {
                    free(argv[0]);
                    argv[0] = NULL;
                    argc[0] = 0;
                }
            } else {
                free(argv[0]);
            }
        }
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    } else {
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }

    return 0;
}

t_pd_err binbuf_get_attribute(t_binbuf* d, t_symbol* key, int* argc, t_atom** argv)
{
    if (d)
        return atoms_get_attribute(binbuf_getnatom(d), binbuf_getvec(d), key, argc, argv);
    else
        return -1;
}

t_pd_err atoms_get_attribute_int(int ac, t_atom* av, t_symbol* key, int* value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if (!atoms_get_attribute(ac, av, key, &argc, &argv)) {
        if (argc && argv) {
            if (atom_gettype(argv) == A_FLOAT) {
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

t_pd_err binbuf_get_attribute_int(t_binbuf* d, t_symbol* key, int* value)
{
    if (d)
        return atoms_get_attribute_int(binbuf_getnatom(d), binbuf_getvec(d), key, value);
    else
        return -1;
}

t_pd_err atoms_get_attribute_long(int ac, t_atom* av, t_symbol* key, long* value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if (!atoms_get_attribute(ac, av, key, &argc, &argv)) {
        if (argc && argv) {
            if (atom_gettype(argv) == A_FLOAT) {
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

t_pd_err binbuf_get_attribute_long(t_binbuf* d, t_symbol* key, long* value)
{
    if (d)
        return atoms_get_attribute_long(binbuf_getnatom(d), binbuf_getvec(d), key, value);
    else
        return -1;
}

t_pd_err atoms_get_attribute_float(int ac, t_atom* av, t_symbol* key, float* value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if (!atoms_get_attribute(ac, av, key, &argc, &argv)) {
        if (argc && argv) {
            if (atom_gettype(argv) == A_FLOAT) {
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

t_pd_err binbuf_get_attribute_float(t_binbuf* d, t_symbol* key, float* value)
{
    if (d)
        return atoms_get_attribute_float(binbuf_getnatom(d), binbuf_getvec(d), key, value);
    else
        return -1;
}

void epd_add_folder(const char* name, const char* folder)
{
}
