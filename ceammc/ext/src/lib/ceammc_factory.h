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

#include <cstdint>
#include <exception>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

#include "ceammc.h"
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

enum ObjectFactoryFlags : uint32_t {
    OBJECT_FACTORY_DEFAULT = 0x0,
    OBJECT_FACTORY_NO_DEFAULT_INLET = 0x1,
    OBJECT_FACTORY_MAIN_SIGNAL_INLET = 0x2,
    OBJECT_FACTORY_NO_BANG = 0x4,
    OBJECT_FACTORY_NO_FLOAT = 0x8,
    OBJECT_FACTORY_NO_SYMBOL = 0x10,
    OBJECT_FACTORY_NO_LIST = 0x20,
    OBJECT_FACTORY_NO_ANY = 0x40,
    OBJECT_FACTORY_NO_TOOLTIPS = 0x80,
    OBJECT_FACTORY_NO_INLET_DISPATCH = 0x100,
    OBJECT_FACTORY_NO_PROP_DISPATCH = 0x200,
    OBJECT_FACTORY_PARSE_ARGS = 0x400, // process positional args
    OBJECT_FACTORY_PARSE_PROPS = 0x800, // process common properties (starting with @)
    OBJECT_FACTORY_PARSE_POS_PROPS = 0x1000, // process positional properties
};

template <typename T>
class ObjectInitT {
public:
    virtual ~ObjectInitT() { }
    virtual void init(T* obj) = 0;
};

template <typename T>
class ObjectFactory {
    ObjectFactory(ObjectFactory&) = delete;
    ObjectFactory& operator=(ObjectFactory) = delete;

public:
    using ObjectProxy = PdObject<T>;
    using ObjectInit = ObjectInitT<T>;
    using ObjectInitPtr = std::unique_ptr<ObjectInit>;

    using PdBangFunction = void (*)(ObjectProxy*);
    using PdFloatFunction = void (*)(ObjectProxy*, t_float);
    using PdSymbolFunction = void (*)(ObjectProxy*, t_symbol*);
    using PdListFunction = void (*)(ObjectProxy*, t_symbol*, int argc, t_atom* argv);
    using PdAnyFunction = void (*)(ObjectProxy*, t_symbol*, int argc, t_atom* argv);

    using MethodPtrList = void (T::*)(t_symbol*, const AtomListView&);
    using MethodListMap = std::unordered_map<t_symbol*, MethodPtrList>;

public:
    ObjectFactory(const char* name, uint32_t flags = OBJECT_FACTORY_DEFAULT)
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

        setFlag(OBJECT_FACTORY_PARSE_ARGS, true);
        setFlag(OBJECT_FACTORY_PARSE_PROPS, true);
        setFlag(OBJECT_FACTORY_PARSE_POS_PROPS, true);

        // add [dump( method to dump to Pd console
        class_addmethod(c, reinterpret_cast<t_method>(dumpMethodList), SymbolTable::instance().s_dump_fn, A_NULL);
        // add [@*?( method to output all properties
        class_addmethod(c, reinterpret_cast<t_method>(queryPropNames), SYM_PROPS_ALL_Q(), A_NULL);
        // direct property get
        ceammc_class_add_propget_fn(c, getProperty);
        // direct property set
        ceammc_class_add_propset_fn(c, setProperty);
        // is base test
        class_addmethod(c, reinterpret_cast<t_method>(isBaseObject), SymbolTable::instance().s_is_base_obj_fn, A_CANT, A_NULL);

        class_name_ = s_name;
        // add to database
        ObjectInfoStorage::instance().addBase(c, reinterpret_cast<t_newmethod>(createObject));
    }

    /** dtor, that finalizes object creation */
    ~ObjectFactory() { finalize(); }

    /** finalizes object creation */
    void finalize()
    {
        if (!(flags_ & OBJECT_FACTORY_NO_BANG))
            setBangFn(processBang);

        if (!(flags_ & OBJECT_FACTORY_NO_FLOAT))
            setFloatFn(processFloat);

        if (!(flags_ & OBJECT_FACTORY_NO_SYMBOL))
            setSymbolFn(processSymbol);

        if (!(flags_ & OBJECT_FACTORY_NO_LIST))
            setListFn(processList);

        if (!(flags_ & OBJECT_FACTORY_NO_ANY))
            setAnyFn(processAny);

        // xlet tooltips()
        if (!(flags_ & OBJECT_FACTORY_NO_TOOLTIPS))
            class_addmethod(class_, (t_method)annotateFn, SymbolTable::instance().s_annotate_fn, A_CANT, 0);
    }

    /** use default pd bang handler */
    void useDefaultPdBangFn() { flags_ |= OBJECT_FACTORY_NO_BANG; }
    /** use default pd float handler */
    void useDefaultPdFloatFn() { flags_ |= OBJECT_FACTORY_NO_FLOAT; }
    /** use default pd symbol handler */
    void useDefaultPdSymbolFn() { flags_ |= OBJECT_FACTORY_NO_SYMBOL; }
    /** use default pd list handler */
    void useDefaultPdListFn() { flags_ |= OBJECT_FACTORY_NO_LIST; }
    /** use default pd any handler */
    void useDefaultPdAnyFn() { flags_ |= OBJECT_FACTORY_NO_ANY; }
    /** do not show xlet tooltips */
    void noTooltips() { flags_ |= OBJECT_FACTORY_NO_TOOLTIPS; }

    /** do not dispatch inlets */
    void noInletsDispatch() { flags_ |= OBJECT_FACTORY_NO_INLET_DISPATCH; }
    /** do not dispatch properties */
    void noPropsDispatch() { flags_ |= OBJECT_FACTORY_NO_PROP_DISPATCH; }

    /** set help name */
    void setHelp(const char* name)
    {
        class_sethelpsymbol(class_, gensym(name));
    }

    /** set bang handler */
    void setBangFn(PdBangFunction fn)
    {
        fn_bang_ = fn;
        class_addbang(class_, reinterpret_cast<t_method>(fn));
    }

    /** set float handler */
    void setFloatFn(PdFloatFunction fn)
    {
        fn_float_ = fn;
        class_doaddfloat(class_, reinterpret_cast<t_method>(fn));
    }

    /** set symbol handler */
    void setSymbolFn(PdSymbolFunction fn)
    {
        fn_symbol_ = fn;
        class_addsymbol(class_, reinterpret_cast<t_method>(fn));
    }

    /** set list handler */
    void setListFn(PdListFunction fn)
    {
        fn_list_ = fn;
        class_addlist(class_, reinterpret_cast<t_method>(fn));
    }

    /** set any handler */
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

    /** add object alias */
    void addAlias(const char* name)
    {
        ObjectInfoStorage::addAlias(name, class_, reinterpret_cast<t_newmethod>(createObject));
    }

    /** adds click support */
    void useClick()
    {
        class_addmethod(class_, reinterpret_cast<t_method>(processClick), gensym("click"),
            A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
    }

    /** adds loadbang reaction */
    void useLoadBang()
    {
        class_addmethod(class_, reinterpret_cast<t_method>(processLoadBang), gensym("loadbang"), A_DEFFLOAT, 0);
    }

    void processData()
    {
        setListFn(processDataFn);
        flags_ |= OBJECT_FACTORY_NO_LIST;
    }

    template <typename... DataT>
    void processData()
    {
        setListFn(processDataTypedFn<DataT...>);
        flags_ |= OBJECT_FACTORY_NO_LIST;
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

    void addInletInfo(const std::string& txt)
    {
        T::addInletInfo(class_, txt);
    }

    void addOutletInfo(const std::string& txt)
    {
        T::addOutletInfo(class_, txt);
    }

    void setInletsInfo(const std::vector<std::string>& l)
    {
        T::setInletsInfo(class_, l);
    }

    void setOutletsInfo(const std::vector<std::string>& l)
    {
        T::setOutletsInfo(class_, l);
    }

    void setXletsInfo(const std::vector<std::string>& ins, const std::vector<std::string>& outs)
    {
        setInletsInfo(ins);
        setOutletsInfo(outs);
    }

    ObjectInfoStorage::Info& info()
    {
        return ObjectInfoStorage::instance().info(class_);
    }

    void setFlag(uint32_t f, bool on)
    {
        if (on)
            flags_ |= f;
        else
            flags_ = (flags_ & (~f));
    }

    bool checkFlag(uint32_t f) const { return flags_ & f; }

    /** parse positional args */
    void parseArgs(bool value = true) { setFlag(OBJECT_FACTORY_PARSE_ARGS, value); }

    /** parse properties */
    void parseProps(bool value = true) { setFlag(OBJECT_FACTORY_PARSE_PROPS, value); }

    /** parse positional props arguments */
    void parsePosProps(bool value = true) { setFlag(OBJECT_FACTORY_PARSE_POS_PROPS, value); }

    /** set parse mode for positional arguments */
    void parseArgsMode(PdArgs::ParseMode mode) { parse_args_mode_ = mode; }

    /** set parse mode for properties */
    void parsePropsMode(PdArgs::ParseMode mode) { parse_props_mode_ = mode; }

    void noArgsAndPropsParse()
    {
        parseArgs(false);
        parseProps(false);
        parsePosProps(false);
    }

    /** default factory object constructor function */
    static void* createObject(t_symbol* name, int argc, t_atom* argv)
    {
        ObjectProxy* x = nullptr;

        try {
            // print deprecation message
            ObjectInfoStorage::Info class_info;
            if (ObjectInfoStorage::instance().find(name, class_info)) {
                if (class_info.deprecated) {
                    LIB_ERR << "object [" << name->s_name << "] is deprecated";
                    auto it = class_info.dict.find("use_instead");
                    if (it != class_info.dict.end())
                        LIB_ERR << " - use [" << it->second << "] instead";
                }
            }

            // alloc pd struct
            x = reinterpret_cast<ObjectProxy*>(pd_new(class_));
            if (x == nullptr)
                throw std::runtime_error("can't allocate memory for object");

            // prepare PdArgs
            PdArgs args(AtomList(argc, argv), class_name_, &x->pd_obj, name);

            args.noDefaultInlet = flags_ & OBJECT_FACTORY_NO_DEFAULT_INLET;
            args.mainSignalInlet = flags_ & OBJECT_FACTORY_MAIN_SIGNAL_INLET;

            args.parseArgsMode = parse_args_mode_;
            args.parsePropsMode = parse_props_mode_;

            args.parseArgs = flags_ & OBJECT_FACTORY_PARSE_ARGS;
            args.parseProps = flags_ & OBJECT_FACTORY_PARSE_PROPS;
            args.parsePosProps = flags_ & OBJECT_FACTORY_PARSE_POS_PROPS;

            // construct ceammc object
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

        // property parsing
        x->impl->parseProperties();

        // some properties (callback) knows their current value only after object creation
        // update this information
        x->impl->updatePropertyDefaults();

        if (initializer_)
            initializer_->init(x->impl);

        // call overloaded init
        x->impl->initDone();

        return x;
    }

    /** default factory object destructor */
    static void deleteObject(ObjectProxy* x)
    {
        delete x->impl;
    }

    /** default factory bang handler */
    static void processBang(ObjectProxy* x)
    {
        x->impl->onBang();
    }

    /** default factory float handler */
    static void processFloat(ObjectProxy* x, t_floatarg f)
    {
        x->impl->onFloat(static_cast<t_float>(f));
    }

    /** default factory symbol handler */
    static void processSymbol(ObjectProxy* x, t_symbol* s)
    {
        x->impl->onSymbol(s);
    }

    /** default factory list handler */
    static void processList(ObjectProxy* x, t_symbol*, int argc, t_atom* argv)
    {
        const AtomListView args(argv, argc);
        x->impl->onList(args);
    }

    /** default factory any handler handler */
    static void processAny(ObjectProxy* x, t_symbol* s, int argc, t_atom* argv)
    {
        const bool do_inlets_dispatch = !(flags_ & OBJECT_FACTORY_NO_INLET_DISPATCH);
        const bool do_props_dispatch = !(flags_ & OBJECT_FACTORY_NO_PROP_DISPATCH);
        const AtomListView args(argv, argc);

        if (do_inlets_dispatch && x->impl->processAnyInlets(s, args))
            return;

        if (do_props_dispatch && x->impl->processAnyProps(s, args))
            return;

        x->impl->onAny(s, args);
    }

    /** default factory click handler handler */
    static void processClick(ObjectProxy* x, t_symbol* /*sel*/,
        t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
    {
        x->impl->onClick(xpos, ypos, shift, ctrl, alt);
    }

    /** default factory loadbang handler */
    static void processLoadBang(ObjectProxy* x, t_floatarg action)
    {
        x->impl->dispatchLoadBang(static_cast<int>(action));
    }

    /** default factory any data handler */
    static void processDataFn(ObjectProxy* x, t_symbol*, int argc, t_atom* argv)
    {
        if (argc == 1 && argv && Atom::is_data(argv)) {
            x->impl->onData(Atom(*argv));
        } else {
            x->impl->onList(AtomListView(argv, argc));
        }
    }

    /** default factory mutiple typed data handler */
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
            x->impl->onList(AtomListView(argv, argc));
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

    static int getProperty(t_object* x, t_symbol* sel, int* argc, t_atom** argv)
    {
        ObjectProxy* z = reinterpret_cast<ObjectProxy*>(x);

        if (!argc) {
            pd_error("[%s] null argc pointer: %s", class_name_->s_name, __FUNCTION__);
            return 0;
        }

        if (!argv) {
            pd_error("[%s] null argv pointer: %s", class_name_->s_name, __FUNCTION__);
            return 0;
        }

        Property* pp = z->impl->property(sel);
        if (!pp)
            return 0;
        else {
            AtomList pvalue = pp->get();
            *argc = pvalue.size();
            *argv = reinterpret_cast<t_atom*>(getbytes(pvalue.size() * sizeof(t_atom)));
            for (size_t i = 0; i < pvalue.size(); i++)
                (*argv)[i] = pvalue.at(i).atom();

            return 1;
        }
    }

    static int setProperty(t_object* x, t_symbol* sel, int argc, t_atom* argv)
    {
        ObjectProxy* z = reinterpret_cast<ObjectProxy*>(x);
        return z->impl->setProperty(sel, AtomListView(argv, argc));
    }

    /* for runtime testing */
    static int isBaseObject(ObjectProxy*) { return 1; }

    static void defaultListMethod(ObjectProxy* x, t_symbol* sel, int argc, t_atom* argv)
    {
        auto it = list_methods_.find(sel);
        if (it == list_methods_.end()) {
            pd_error(x, "unknown method: %s", sel->s_name);
            return;
        }

        (x->impl->*(it->second))(sel, AtomListView(argv, argc));
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

    static uint32_t flags() { return flags_; }

    /**
     * Sets object initializer
     */
    static void setObjectInit(ObjectInitPtr&& ptr)
    {
        initializer_ = std::move(ptr);
    }

    static void useProxyBang()
    {
        InletProxy<T>::init();
        InletProxy<T>::set_bang_callback(&T::onProxyBang);
    }

    static void useProxyFloat()
    {
        InletProxy<T>::init();
        InletProxy<T>::set_float_callback(&T::onProxyFloat);
    }

    static void useProxySymbol()
    {
        InletProxy<T>::init();
        InletProxy<T>::set_symbol_callback(&T::onProxySymbol);
    }

    static void useProxyList()
    {
        InletProxy<T>::init();
        InletProxy<T>::set_list_callback(&T::onProxyList);
    }

    static void useProxyAny()
    {
        InletProxy<T>::init();
        InletProxy<T>::set_any_callback(&T::onProxyAny);
    }

    static void addProxyMethod(t_symbol* m, typename InletProxy<T>::MethodPtr fn)
    {
        InletProxy<T>::init();
        InletProxy<T>::set_method_callback(m, fn);
    }

private:
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

    /**
     * @brief annotateFn
     * @param x - pd object
     * @param type - 1 (inlet), 0 (outlet)
     * @param idx - inlet index
     * @return
     */
    static const char* annotateFn(ObjectProxy* x, XletType type, int idx)
    {
        switch (type) {
        case XLET_OUT:
            return x->impl->annotateOutlet(idx);
        case XLET_IN:
            return x->impl->annotateInlet(idx);
        default:
            return nullptr;
        }
    }

private:
    static t_class* class_;
    static t_symbol* class_name_;

    static MethodListMap list_methods_;
    static uint32_t flags_;
    static PdArgs::ParseMode parse_args_mode_;
    static PdArgs::ParseMode parse_props_mode_;

    static ObjectInitPtr initializer_;

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
t_class* ObjectFactory<T>::class_ = nullptr;

template <typename T>
t_symbol* ObjectFactory<T>::class_name_ = 0;

template <typename T>
typename ObjectFactory<T>::MethodListMap ObjectFactory<T>::list_methods_;

template <typename T>
uint32_t ObjectFactory<T>::flags_ = 0;

template <typename T>
PdArgs::ParseMode ObjectFactory<T>::parse_args_mode_ = PdArgs::PARSE_EXPR;

template <typename T>
PdArgs::ParseMode ObjectFactory<T>::parse_props_mode_ = PdArgs::PARSE_EXPR;

template <typename T>
typename ObjectFactory<T>::ObjectInitPtr ObjectFactory<T>::initializer_;

#define CLASS_ADD_METHOD()

} // namespace ceammc

#endif // CEAMMC_OBJECTCLASS_H
