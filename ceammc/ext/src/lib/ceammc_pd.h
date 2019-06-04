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
#include "ceammc_property_info.h"

#include <map>
#include <memory>
#include <string>

struct _text;
typedef struct _text t_object;

namespace ceammc {

class Canvas;
class BaseObject;
class UIObject;

namespace pd {
    class External {
    private:
        t_object* obj_;

    public:
        External(const char* name, const AtomList& lst = AtomList());
        ~External();

        bool isNull() const;

        // connect this object outlet to external object inlet
        bool connectTo(int outn, t_object* dest, int inln);
        bool connectTo(int outn, External& ext, int inln);

        // connect external source outlet to this object inlet
        bool connectFrom(int outn, t_object* src, int inln);
        bool connectFrom(int outn, External& ext, int inln);

        t_object* object();
        t_pd* pd() { return &obj_->te_g.g_pd; }

        void sendBang();
        void sendFloat(t_float v);
        void sendSymbol(t_symbol* s);
        void sendList(const AtomList& l);
        void sendBangTo(size_t inlet);
        void sendFloatTo(t_float v, size_t inlet);
        void sendSymbolTo(t_symbol* s, size_t inlet);
        void sendListTo(const AtomList& l, size_t inlet);
        void sendMessage(t_symbol* msg, const AtomList& args = AtomList());

        int numOutlets() const;
        int numInlets() const;

        int xPos() const;
        int yPos() const;
        void setXPos(int x);
        void setYPos(int y);

        std::vector<t_symbol*> methods() const;

        bool isCeammc() const;
        bool isCeammcBase() const;
        bool isCeammcUI() const;
        bool isCeammcFaust() const;
        bool isCeammcFlext() const;

        const BaseObject* asCeammcBaseObject() const;
        const UIObject* asCeammcUIObject() const;

        std::vector<PropertyInfo> properties() const;
    };
}

typedef std::shared_ptr<Canvas> CanvasPtr;
typedef std::map<std::string, CanvasPtr> CanvasMap;

class PureData {
    CanvasMap canvas_map_;

public:
    static PureData& instance();
    CanvasPtr createTopCanvas(const char* name, const AtomList& args = AtomList());
    CanvasPtr createSubpatch(_glist* parent, const char* name);

private:
    PureData();
};
}

#endif // CEAMMC_PD_H
