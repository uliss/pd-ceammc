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
#ifndef CEAMMC_PD_H
#define CEAMMC_PD_H

#include "ceammc_atomlist.h"
#include "ceammc_canvas.h"

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

struct _text;
typedef struct _text t_object;

namespace ceammc {
namespace pd {
    class External {
    private:
        t_object* obj_;

    public:
        External(const char* name, const AtomList& lst = AtomList());
        ~External();

        bool isNull() const;

        // connect this object outlet to external object ilnet
        bool connectTo(int outn, t_object* dest, int inln);
        bool connectTo(int outn, External& ext, int inln);

        t_object* object();

        void bang();
        void sendFloat(t_float v);
    };
}

typedef boost::shared_ptr<Canvas> CanvasPtr;
typedef std::map<std::string, CanvasPtr> CanvasMap;

class PureData {
    CanvasMap canvas_map_;

public:
    static PureData& instance();
    CanvasPtr createTopCanvas(const char* name);

private:
    PureData();
};
}

#endif // CEAMMC_PD_H
