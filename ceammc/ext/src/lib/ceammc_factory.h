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

#include "m_pd.h"

#include <exception>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

#include "ceammc_data.h"
#include "ceammc_message.h"
#include "ceammc_object.h"
#include "ceammc_object_info.h"

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

    typedef void (T::*MethodPtrList)(t_symbol*, const AtomList&);
    typedef std::unordered_map<t_symbol*, MethodPtrList> MethodListMap;

public:
    ObjectFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
        : fn_bang_(nullptr)
        , fn_float_(nullptr)
        , fn_symbol_(nullptr)
        , fn_list_(nullptr)
        , fn_any_(nullptr)
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

        // add [dump( method to dump to Pd console
        class_addmethod(c, reinterpret_cast<t_method>(dumpMethodList), SYM_DUMP(), A_NULL);
        // add [@*?( method to output all properties
        class_addmethod(c, reinterpret_cast<t_method>(queryPropNames), SYM_PROPS_ALL_Q(), A_NULL);

        class_name_ = s_name;
        // add to database
        ObjectInfoStorage::instance().addBase(c);
    }

    void mapFloatToList()
    {
        fn_float_ = defaultFloatToList;
        class_doaddfloat(class_, reinterpret_cast<t_method>(fn_float_));
    }

    void mapSymbolToList()
    {
        fn_symbol_ = defaultSymbolToList;
        class_addsymbol(class_, reinterpret_cast<t_method>(fn_symbol_));
    }

    void setHelp(const char* name)
    {
        class_sethelpsymbol(class_, gensym(name));
    }

    void setBangFn(PdBangFunction fn)
    {
        fn_bang_ = fn;
        class_addbang(class_, reinterpret_cast<t_method>(fn));
    }

    void setFloatFn(PdFloatFunction fn)
    {
        fn_float_ = fn;
        class_doaddfloat(class_, reinterpret_cast<t_method>(fn));
    }

    void setSymbolFn(PdSymbolFunction fn)
    {
        fn_symbol_ = fn;
        class_addsymbol(class_, reinterpret_cast<t_method>(fn));
    }

    void setListFn(PdListFunction fn)
    {
        fn_list_ = fn;
        class_addlist(class_, reinterpret_cast<t_method>(fn));
    }

    void setAnyFn(PdAnyFunction fn)
    {
        fn_any_ = fn;
        class_addanything(class_, reinterpret_cast<t_method>(fn));
    }

    void addMethod(const char* name, MethodPtrList fn)
    {
        t_symbol* s = gensym(name);
        class_addmethod(class_, reinterpret_cast<t_method>(defaultListMethod), s, A_GIMME, A_NULL);
        list_methods_[s] = fn;
    }

    void addAlias(const char* name)
    {
        class_addcreator(reinterpret_cast<t_newmethod>(createObject), gensym(name), A_GIMME, A_NULL);
        ObjectInfoStorage::instance().info(class_).aliases.push_back(name);
    }

    void useClick()
    {
        class_addmethod(class_, reinterpret_cast<t_method>(processClick), gensym("click"),
            A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
    }

    void useLoadBang()
    {
        class_addmethod(class_, reinterpret_cast<t_method>(processLoadBang), gensym("loadbang"), A_DEFFLOAT, 0);
    }

    void processData()
    {
        setListFn(processDataFn);
    }

    template <typename... DataT>
    void processData()
    {
        setListFn(processDataTypedFn<DataT...>);
    }

    void setDescription(const std::string& str)
    {
        ObjectInfoStorage::instance().info(class_).dict["description"] = str;
    }

    void setCategory(const std::string& str)
    {
        ObjectInfoStorage::instance().info(class_).dict["category"] = str;
    }

    void addAuthor(const std::string& name)
    {
        ObjectInfoStorage::instance().info(class_).authors.push_back(name);
    }

    void setSinceVersion(uint16_t major, uint16_t minor)
    {
        ObjectInfoStorage::instance().info(class_).since_version = ObjectInfoStorage::Version(major, minor);
    }

    void setDeprecated()
    {
        ObjectInfoStorage::instance().info(class_).deprecated = true;
    }

    void setUseInstead(const std::string& name)
    {
        ObjectInfoStorage::instance().info(class_).dict["use_instead"] = name;
    }

    void setKeywords(const std::vector<std::string>& l)
    {
        ObjectInfoStorage::instance().info(class_).keywords = l;
    }

    void setApiVersion(uint16_t v)
    {
        ObjectInfoStorage::instance().info(class_).api = v;
    }

    static void* createObject(t_symbol* name, int argc, t_atom* argv)
    {
        ObjectProxy* x = nullptr;

        try {
            x = reinterpret_cast<ObjectProxy*>(pd_new(class_));
            if (x == nullptr)
                throw std::runtime_error("can't allocate memory for object");

            PdArgs args(AtomList(argc, argv), class_name_, &x->pd_obj, name);
            args.noDefaultInlet = flags_ & OBJECT_FACTORY_NO_DEFAULT_INLET;
            args.mainSignalInlet = flags_ & OBJECT_FACTORY_MAIN_SIGNAL_INLET;

            x->impl = new T(args);
        } catch (std::exception& e) {
            pd_error(0, "[ceammc] can't create object [%s]: %s", class_name_->s_name, e.what());

            x->impl = nullptr;
            pd_free(&x->pd_obj.te_g.g_pd);
            return nullptr;
        } catch (...) {
            pd_error(0, "[ceammc] can't create object [%s]", class_name_->s_name);

            x->impl = nullptr;
            pd_free(&x->pd_obj.te_g.g_pd);
            return nullptr;
        }

        x->impl->parseProperties();
        x->impl->initDone();

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
        x->impl->onFloat(static_cast<t_float>(f));
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

    static void processClick(ObjectProxy* x, t_symbol* /*sel*/,
        t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
    {
        x->impl->onClick(xpos, ypos, shift, ctrl, alt);
    }

    static void processLoadBang(ObjectProxy* x, t_floatarg action)
    {
        x->impl->dispatchLoadBang(static_cast<int>(action));
    }

    static void processDataFn(ObjectProxy* x, t_symbol*, int argc, t_atom* argv)
    {
        if (argc == 1 && argv && Atom::is_data(argv)) {
            x->impl->onData(Atom(*argv));
        } else {
            x->impl->onList(AtomList(argc, argv));
        }
    }

    //    template <typename DataT>
    //    static void processDataTypedFn(ObjectProxy* x, t_symbol*, int argc, t_atom* argv)
    //    {
    //        if (argc == 1 && argv && Atom::is_data(argv)) {
    //            Atom data(*argv);
    //            if (data.isA<DataT>())
    //                x->impl->onDataT(DataAtom<DataT>(std::move(data)));
    //            else
    //                x->impl->onData(data);
    //        } else {
    //            x->impl->onList(AtomList(argc, argv));
    //        }
    //    }

    template <typename DataT>
    static bool processDataSingleTypedFn(ObjectProxy* x, const Atom& a)
    {
        if (a.isA<DataT>()) {
            x->impl->onDataT(DataAtom<DataT>(a));
            return true;
        } else
            return false;
    }

    struct utility {
        template <typename TypeList, int index>
        struct iterate_pred {
            static bool next(ObjectProxy* x, const Atom& a)
            {
                using Type = typename std::tuple_element<index - 1, TypeList>::type;
                if (iterate_pred<TypeList, index - 1>::next(x, a))
                    return true;
                else
                    return processDataSingleTypedFn<Type>(x, a);
            }
        };

        template <typename TypeList>
        struct iterate_pred<TypeList, 0> {
            static bool next(ObjectProxy* x, const Atom& a)
            {
                using Type = typename std::tuple_element<0, TypeList>::type;
                return processDataSingleTypedFn<Type>(x, a);
            }
        };
    };

    template <typename... Types>
    static void processDataTypedFn(ObjectProxy* x, t_symbol*, int argc, t_atom* argv)
    {
        using TypeList = std::tuple<Types...>;
        constexpr int NumTypes = std::tuple_size<TypeList>::value;
        using IteratorPred = typename utility::template iterate_pred<TypeList, NumTypes>;

        if (argc == 1 && Atom::is_data(*argv)) {
            Atom data(*argv);
            if (!IteratorPred::next(x, data))
                x->impl->onData(data);
        } else {
            x->impl->onList(AtomList(argc, argv));
        }
    }

    static void dumpMethodList(ObjectProxy* x)
    {
        for (auto& m : list_methods_) {
            post("[%s] method: %s", class_name_->s_name, m.first->s_name);
        }

        x->impl->dump();
    }

    static void queryPropNames(ObjectProxy* x)
    {
        x->impl->queryPropNames();
    }

    static void defaultListMethod(ObjectProxy* x, t_symbol* sel, int argc, t_atom* argv)
    {
        auto it = list_methods_.find(sel);
        if (it == list_methods_.end()) {
            pd_error(x, "unknown method: %s", sel->s_name);
            return;
        }

        (x->impl->*(it->second))(sel, AtomList(argc, argv));
    }

    /**
     * @brief classPointer
     * @return pointer to Pd class
     */
    static t_class* classPointer() { return class_; }

    /**
     * @brief className
     * @return Pd class name
     */
    static t_symbol* className() { return class_name_; }

    /**
     * convert from Pd object pointer to pointer to ceammc class
     * @param x - pd object pointer
     * @return - pointer to ceammc object or nullptr on error
     */
    static T* fromObject(t_object* x)
    {
        if (!x)
            return nullptr;

        return reinterpret_cast<ObjectProxy*>(x)->impl;
    }

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
    static MethodListMap list_methods_;
    static int flags_;

private:
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

            using Object = typename SoundExternalFactory::ObjectProxy;
            int offset = static_cast<int>(offsetof(Object, f));
            class_domainsignalin(SoundExternalFactory::classPointer(), offset);
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
typename ObjectFactory<T>::MethodListMap ObjectFactory<T>::list_methods_;

template <typename T>
int ObjectFactory<T>::flags_ = 0;

#define CLASS_ADD_METHOD()

} // namespace ceammc

#endif // CEAMMC_OBJECTCLASS_H
