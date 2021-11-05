/*****************************************************************************
 *
 * Created: 17.10.2016 2016 by Serge Poltavski
 *
 * Copyright 2016 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef CEAMMC_H
#define CEAMMC_H

#ifdef __WIN32__
#define CEAMMC_EXTERN __declspec(dllexport)
#else
#define CEAMMC_EXTERN
#endif

#include "m_pd.h"

#include <stdexcept>
#include <string>

namespace ceammc {

enum XletType : int {
    XLET_IN = 1,
    XLET_OUT = 0
};

using PropertyGetFn = int (*)(t_object* x, t_symbol* prop_name, int* argc, t_atom** argv);
using PropertySetFn = int (*)(t_object* x, t_symbol* prop_name, int argc, t_atom* argv);
using PropertyFindFn = void* (*)(t_object* x, t_symbol* prop_name);
using XletGetAnnotationFn = const char* (*)(t_object* object, XletType type, int xlet_idx);
using XletGetTclIdFn = void (*)(t_glist* glist, void* object, char* buf, size_t bufsize, XletType type, int xlet_idx);

struct OutletIdx {
    unsigned long long n;

    operator unsigned long long() const { return n; }
};

struct InletIdx {
    unsigned long long n;

    operator unsigned long long() const { return n; }
};

class SymbolTable {
    SymbolTable();
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;

public:
    static const SymbolTable& instance();

    // internal
    t_symbol* s_annotate_fn;
    t_symbol* s_propget_fn;
    t_symbol* s_propset_fn;
    t_symbol* s_propfind_fn;

    // tests
    t_symbol* s_is_base_obj_fn;
    t_symbol* s_is_cicm_obj_fn;
    t_symbol* s_is_flext_obj_fn;

    // common functions
    t_symbol* s_dump_fn;
};

namespace literals {
    inline OutletIdx operator"" _out(unsigned long long n) { return { n }; }
    inline InletIdx operator"" _in(unsigned long long n) { return { n }; }
}

struct Exception : public std::runtime_error {
public:
    Exception(const char* msg)
        : std::runtime_error(msg)
    {
    }

    Exception(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

class InvalidOutlet : public Exception {
    OutletIdx n_;

public:
    InvalidOutlet(OutletIdx n) noexcept;
    const char* what() const noexcept;
};

void ceammc_tcl_init_tooltips();
void ceammc_xlet_bind_tooltip(t_object* x, t_glist* glist, XletType type, const char* xlet_id, const char* txt);
void ceammc_xlet_bind_tooltip(t_object* x, t_glist* glist, XletGetTclIdFn id_fn, XletGetAnnotationFn ann_fn, XletType type, int xlet_idx);
void ceammc_class_add_propget_fn(t_class* c, PropertyGetFn fn);
void ceammc_class_add_propset_fn(t_class* c, PropertySetFn fn);

PropertyGetFn ceammc_get_propget_fn(t_object* x);
PropertySetFn ceammc_get_propset_fn(t_object* x);
PropertyFindFn ceammc_get_propfind_fn(t_object* x);

/**
 * Returns pointer to annotation function or nullptr if not found
 * @param x - point to object
 */
XletGetAnnotationFn ceammc_get_annotation_fn(t_pd* x);

}

#endif // CEAMMC_H
