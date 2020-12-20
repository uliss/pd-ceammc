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
#include "ceammc_message.h"
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

    struct XletInfo {
        enum Type {
            NONE,
            CONTROL,
            SIGNAL
        };

        Type type = { NONE };
        XletInfo(Type t)
            : type(t)
        {
        }
        bool isSignal() const { return type == SIGNAL; }
    };

    std::vector<std::string> currentListOfExternals();

    bool addPdPrintDataSupport();

    class External {
    private:
        t_object* obj_;
        _glist* parent_;

    public:
        External(const char* name, const AtomList& lst = AtomList());

        template <typename... Args>
        External(const char* name, Args... args)
            : External(name, AtomList(args...))
        {
        }

        ~External();

        bool isNull() const;

        t_symbol* className() const;

        // connect this object outlet to external object inlet
        bool connectTo(size_t outn, t_object* dest, size_t inln);
        bool connectTo(size_t outn, External& ext, size_t inln);

        // connect external source outlet to this object inlet
        bool connectFrom(size_t outn, t_object* src, size_t inln);
        bool connectFrom(size_t outn, External& ext, size_t inln);

        t_object* object();
        const t_object* object() const;
        t_pd* pd() { return &obj_->te_g.g_pd; }
        void setParent(_glist* cnv);

        void sendBang();
        void sendFloat(t_float v);
        void sendSymbol(t_symbol* s);
        void sendSymbol(const char* s) { sendSymbol(gensym(s)); }
        void sendList(const AtomList& l);
        template <typename... Args>
        void sendList(Args... args)
        {
            sendList(AtomList(args...));
        }

        void sendBangTo(size_t inlet);
        void sendFloatTo(t_float v, size_t inlet);
        void sendSymbolTo(t_symbol* s, size_t inlet);
        void sendListTo(const AtomList& l, size_t inlet);
        void sendMessage(t_symbol* msg, const AtomList& args = AtomList());
        void sendMessage(const Message& m);
        void sendMessageTo(const Message& m, size_t inlet);

        template <typename... Args>
        void sendMessage(const char* msg, Args... args)
        {
            sendMessage({ msg, args... });
        }

        void sendMessage(const char* msg, const AtomList& args = AtomList())
        {
            sendMessage(gensym(msg), args);
        }

        int numOutlets() const;
        int numInlets() const;

        XletInfo inletInfo(int i) const;
        XletInfo outletInfo(int i) const;

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

    t_class* object_class(t_object* x);
    t_symbol* object_name(t_object* x);
    t_symbol* object_dir(t_object* x);
    void object_bang(t_object* x);
}

typedef std::shared_ptr<Canvas> CanvasPtr;
typedef std::map<std::string, CanvasPtr> CanvasMap;

class PureData {
    CanvasMap canvas_map_;

public:
    static PureData& instance();
    CanvasPtr createTopCanvas(const char* name, const AtomList& args = AtomList());
    CanvasPtr createSubpatch(_glist* parent, const char* name);
    CanvasPtr findCanvas(const char* name);

    PureData(const PureData&) = delete;
    PureData& operator=(const PureData&) = delete;

private:
    PureData();
};
}

#endif // CEAMMC_PD_H
