/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_CANVAS_H
#define CEAMMC_CANVAS_H

#include "ceammc_array.h"

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

struct _glist;
struct _symbol;
struct _text;
typedef struct _text t_object;

namespace ceammc {

class BaseObject;
typedef boost::shared_ptr<Array> ArrayPtr;
typedef std::map<_symbol*, ArrayPtr> ArrayMap;

class Canvas {
    ArrayMap array_list_;
    _glist* canvas_;

public:
    Canvas(_glist* t);

    ArrayPtr createArray(const char* name, size_t n);

    static bool connect(t_object* src, size_t nout, t_object* dest, size_t ninl);
    static bool connect(const BaseObject& src, size_t nout, BaseObject& dest, size_t ninl);
};
}

#endif // CEAMMC_CANVAS_H
