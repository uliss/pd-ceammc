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
#ifndef CEAMMC_OBJECTCLASS_H
#define CEAMMC_OBJECTCLASS_H

#include <m_pd.h>

#include <exception>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "ceammc_message.h"
#include "ceammc_object.h"

namespace ceammc {

template <typename T>
class ObjectFactory;

template <typename T>
struct PdObject {
    t_object pd_obj;
    T* impl;
    t_sample f;
};

enum ObjectFactoryFlags {
    OBJECT_FACTORY_DEFAULT = 0x0,
    OBJECT_FACTORY_NO_DEFAULT_INLET = 0x1,
    OBJECT_FACTORY_MAIN_SIGNAL_INLET = 0x2,
    OBJECT_FACTORY_NO_BANG = 0x4,
    OBJECT_FACTORY_NO_FLOAT = 0x8,
    OBJECT_FACTORY_NO_SYMBOL = 0x10,
    OBJECT_FACTORY_NO_LIST = 0x20,
    OBJECT_FACTORY_NO_ANY = 0x40
};

template <typename T>
class ObjectFactory {
public:
    typedef PdObject<T> ObjectProxy;

    typedef void (*PdBangFunction)(ObjectProxy*);
    typedef void (*PdFloatFunction)(ObjectProxy*, t_float);
    typedef void (*PdSymbolFunction)(ObjectProxy*, t_symbol*);
    typedef void (*PdListFunction)(ObjectProxy*, t_symbol*, int argc, t_atom* argv);
    typedef void (*PdAnyFunction)(ObjectProxy*, t_symbol*, int argc, t_atom* argv);

    typedef void (T::*MethodPtrBang)();
    typedef void (T::*MethodPtrFloat)(float);
    typedef void (T::*MethodPrtSymbol)(t_symbol*);
    typedef void (T::*MethodPtrList)(t_symbol* s, const AtomList& l);

    typedef std::map<t_symbol*, MethodPtrBang> MethodBangMap;
    typedef std::map<t_symbol*, MethodPtrFloat> MethodFloatMap;
    typedef std::map<t_symbol*, MethodPrtSymbol> MethodSymbolMap;
    typedef std::map<t_symbol*, MethodPtrList> MethodListMap;

public:
    ObjectFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
        : name_(name)
        , fn_bang_(0)
        , fn_float_(0)
        , fn_symbol_(0)
        , fn_list_(0)
        , fn_any_(0)
    {
        int pd_flags = CLASS_PATCHABLE;
        if (flags & OBJECT_FACTORY_NO_DEFAULT_INLET) {
            pd_flags |= CLASS_NOINLET;
            if (flags & OBJECT_FACTORY_MAIN_SIGNAL_INLET) {
                LIB_ERR << name << ": invalid flag combination - "
                                   "OBJECT_FACTORY_NO_DEFAULT_INLET & OBJECT_FACTORY_MAIN_SIGNAL_INLET";
            }
        }

        t_symbol* s_name = gensym(name);
        t_class* c = class_new(s_name,
            reinterpret_cast<t_newmethod>(createObject),
            reinterpret_cast<t_method>(deleteObject),
            sizeof(ObjectProxy), pd_flags, A_GIMME, A_NULL);

        class_ = c;
        flags_ = flags;

        if (!(flags & OBJECT_FACTORY_NO_BANG))
            setBangFn(processBang);

        if (!(flags & OBJECT_FACTORY_NO_FLOAT))
            setFloatFn(processFloat);

        if (!(flags & OBJECT_FACTORY_NO_SYMBOL))
            setSymbolFn(processSymbol);

        if (!(flags & OBJECT_FACTORY_NO_LIST))
            setListFn(processList);

        if (!(flags & OBJECT_FACTORY_NO_ANY))
            setAnyFn(processAny);

        class_addmethod(c, reinterpret_cast<t_method>(dumpMethodList), gensym("dump"), A_NULL);

        class_name_ = s_name;
    }

    void mapFloatToList()
    {
        fn_float_ = defaultFloatToList;
        class_addfloat(class_, fn_float_);
    }

    void mapSymbolToList()
    {
        fn_symbol_ = defaultSymbolToList;
        class_addsymbol(class_, fn_symbol_);
    }

    void setHelp(const char* name)
    {
        class_sethelpsymbol(class_, gensym(name));
    }

    void setBangFn(PdBangFunction fn)
    {
        fn_bang_ = fn;
        class_addbang(class_, fn);
    }

    void setFloatFn(PdFloatFunction fn)
    {
        fn_float_ = fn;
        class_addfloat(class_, fn);
    }

    void setSymbolFn(PdSymbolFunction fn)
    {
        fn_symbol_ = fn;
        class_addsymbol(class_, fn);
    }

    void setListFn(PdListFunction fn)
    {
        fn_list_ = fn;
        class_addlist(class_, fn);
    }

    void setAnyFn(PdAnyFunction fn)
    {
        fn_any_ = fn;
        class_addanything(class_, fn);
    }

    void addMethod(const char* name, MethodPtrList fn)
    {
        t_symbol* s = gensym(name);
        class_addmethod(class_, reinterpret_cast<t_method>(defaultListMethod), s, A_GIMME, A_NULL);
        methods_[s] = fn;
    }

    void addAlias(const char* name)
    {
        class_addcreator(reinterpret_cast<t_newmethod>(createObject), gensym(name), A_GIMME, A_NULL);
    }

    void processData()
    {
        setListFn(processDataFn);
    }

    template <class DataT>
    void processData()
    {
        setListFn(processDataTypedFn<DataT>);
    }

    static void* createObject(t_symbol*, int argc, t_atom* argv)
    {
        ObjectProxy* x = 0;
        try {
            x = reinterpret_cast<ObjectProxy*>(pd_new(class_));

            PdArgs args(AtomList(argc, argv), class_name_, &x->pd_obj);
            args.noDefaultInlet = flags_ & OBJECT_FACTORY_NO_DEFAULT_INLET;
            args.mainSignalInlet = flags_ & OBJECT_FACTORY_MAIN_SIGNAL_INLET;

            x->impl = new T(args);
            x->impl->parseProperties();
        } catch (std::exception& e) {
            x->impl = 0;
            pd_free(&x->pd_obj.te_g.g_pd);
            x = 0;

            char buf[200];
            snprintf(buf, 199, "%s", e.what());
            pd_error(0, "[ceammc] can't create object [%s]: %s", class_name_->s_name, buf);
        } catch (...) {
            x->impl = 0;
            pd_free(&x->pd_obj.te_g.g_pd);
            x = 0;

            pd_error(0, "[ceammc] can't create object [%s]", class_name_->s_name);
        }

        return x;
    }

    static void deleteObject(ObjectProxy* x)
    {
        delete x->impl;
    }

    static void processBang(ObjectProxy* x)
    {
        x->impl->onBang();
    }

    static void processFloat(ObjectProxy* x, t_floatarg f)
    {
        x->impl->onFloat(static_cast<double>(f));
    }

    static void processSymbol(ObjectProxy* x, t_symbol* s)
    {
        x->impl->onSymbol(s);
    }

    static void processList(ObjectProxy* x, t_symbol*, int argc, t_atom* argv)
    {
        x->impl->onList(AtomList(argc, argv));
    }

    static void processAny(ObjectProxy* x, t_symbol* s, int argc, t_atom* argv)
    {
        x->impl->anyDispatch(s, AtomList(argc, argv));
    }

    static void processDataFn(ObjectProxy* x, t_symbol*, int argc, t_atom* argv)
    {
        AtomList l(argc, argv);
        if (l.size() == 1 && l[0].isData()) {
            DataPtr ptr(l[0]);
            if (ptr.isValid()) {
                x->impl->onData(ptr);
            } else {
                DataDesc desc = l[0].getData();
                LIB_ERR << "can't get data with type=" << desc.type << " and id=" << desc.id;
            }
        } else {
            x->impl->onList(l);
        }
    }

    template <class DataT>
    static void processDataTypedFn(ObjectProxy* x, t_symbol*, int argc, t_atom* argv)
    {
        AtomList l(argc, argv);
        if (l.size() == 1 && l[0].isData()) {
            DataTPtr<DataT> ptr(l[0]);
            if (ptr.isValid()) {
                x->impl->onDataT(*ptr.data());
            } else {
                DataDesc d = l[0].getData();
                LIB_ERR << "can't get data with type=" << d.type << " and id=" << d.id;
            }
        } else {
            x->impl->onList(l);
        }
    }

    static void dumpMethodList(ObjectProxy* x)
    {
        typename MethodListMap::iterator it;
        for (it = methods_.begin(); it != methods_.end(); ++it) {
            post("[%s] method: %s", class_name_->s_name, it->first->s_name);
        }

        x->impl->dump();
    }

    static void defaultListMethod(ObjectProxy* x, t_symbol* sel, int argc, t_atom* argv)
    {
        typename MethodListMap::iterator it = methods_.find(sel);
        if (it == methods_.end()) {
            pd_error(x, "unknown method: %s", sel->s_name);
            return;
        }

        (x->impl->*(it->second))(sel, AtomList(argc, argv));
    }

    static t_class* classPointer() { return class_; }

private:
    static void defaultFloatToList(ObjectProxy* x, t_floatarg f)
    {
        x->impl->onList(AtomList::filled(f, 1));
    }

    static void defaultSymbolToList(ObjectProxy* x, t_symbol* s)
    {
        x->impl->onList(listFrom(s));
    }

private:
    static t_class* class_;
    static t_symbol* class_name_;
    static MethodListMap methods_;
    static int flags_;

private:
    const char* name_;
    PdBangFunction fn_bang_;
    PdFloatFunction fn_float_;
    PdSymbolFunction fn_symbol_;
    PdListFunction fn_list_;
    PdAnyFunction fn_any_;
};

template <typename T>
class SoundExternalFactory : public ObjectFactory<T> {
public:
    SoundExternalFactory(const char* name,
        int flags = OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_MAIN_SIGNAL_INLET | OBJECT_FACTORY_NO_FLOAT)
        : ObjectFactory<T>(name, flags)
    {
        class_addmethod(SoundExternalFactory::classPointer(),
            reinterpret_cast<t_method>(setupDSP), gensym("dsp"), A_NULL);

        // if default inlet is signal
        if (!(flags & OBJECT_FACTORY_NO_DEFAULT_INLET)
            && (flags & OBJECT_FACTORY_MAIN_SIGNAL_INLET)) {

            CLASS_MAINSIGNALIN(SoundExternalFactory::classPointer(),
                typename SoundExternalFactory::ObjectProxy, f);
        }
    }

    static void setupDSP(typename SoundExternalFactory::ObjectProxy* x, t_signal** sp)
    {
        x->impl->setupDSP(sp);
    }
};

template <typename T>
t_class* ObjectFactory<T>::class_;

template <typename T>
t_symbol* ObjectFactory<T>::class_name_ = 0;

template <typename T>
typename ObjectFactory<T>::MethodListMap ObjectFactory<T>::methods_;

template <typename T>
int ObjectFactory<T>::flags_ = 0;

#define CLASS_ADD_METHOD()

} // namespace ceammc

#endif // CEAMMC_OBJECTCLASS_H
