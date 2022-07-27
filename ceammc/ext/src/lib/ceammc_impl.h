/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_IMPL_H
#define CEAMMC_IMPL_H

struct _class;
struct _symbol;
struct _atom;
struct _methodentry;

namespace ceammc {

typedef void (*PdBangFunction)(_class*);
typedef void (*PdFloatFunction)(_class*, float);
typedef void (*PdSymbolFunction)(_class*, _symbol*);
typedef void (*PdListFunction)(_class*, _symbol*, int argc, _atom* argv);
typedef void (*PdAnyFunction)(_class*, _symbol*, int argc, _atom* argv);

PdBangFunction default_pd_bang_fn(_class* c);
PdFloatFunction default_pd_float_fn(_class* c);

/**
 * get class method name by given index
 * @param c - pointer to class
 * @param method_idx - method index
 * @return method name or NULL on error
 */
_symbol* class_method_name(_class* c, int method_idx);

/**
 * runtime check if external running under CEAMMC distribution
 */
bool is_ceammc_distribution();



} // namespace ceammc

#endif // CEAMMC_IMPL_H
