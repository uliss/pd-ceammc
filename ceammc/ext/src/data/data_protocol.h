/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef DATA_PROTOCOL_H
#define DATA_PROTOCOL_H

#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_object.h"
#include "ceammc_platform.h"
#include "ceammc_property_callback.h"

#include <fstream>

namespace ceammc {

template <typename T>
class BaseIFace : public T {
public:
    BaseIFace(const PdArgs& args)
        : T(args)
    {
        static_assert(std::is_base_of<BaseObject, T>::value, "need to be ancestor of BaseObject");
    }

    void m_set(t_symbol* /*s*/, const AtomListView& lv)
    {
        proto_set(lv);
    }

    virtual void proto_set(const AtomListView&) = 0;
};

template <typename T, typename NumType>
class NumericIFace : public BaseIFace<T> {
public:
    NumericIFace(const PdArgs& args)
        : BaseIFace<T>(args)
    {
        using F = std::function<NumType()>;
        T::addProperty(new CallbackProperty("@value", F([this]() -> NumType { return value(); }), nullptr));
    }

    void onBang() override
    {
        this->floatTo(0, static_cast<NumType>(value()));
    }

    void onFloat(t_float f) override
    {
        value() = static_cast<NumType>(f);
        onBang();
    }

    void onList(const AtomListView& lv) override
    {
        if (lv.empty()) {
            OBJ_ERR << "empty list";
            return;
        }

        if (!lv[0].isFloat()) {
            OBJ_ERR << "non a number: " << lv[0];
            return;
        }

        onFloat(lv[0].asT<t_float>());
    }

    void onInlet(size_t /*n*/, const AtomListView& lv) override
    {
        proto_set(lv);
    }

    AtomList propValue() const
    {
        return Atom(value());
    }

    void m_plus(t_symbol* s, const AtomListView& lv)
    {
        if (!T::checkArgs(lv, T::ARG_FLOAT) || lv.size() != 1) {
            METHOD_ERR(s) << "single numeric argument expected: " << lv;
            return;
        }

        value() += NumType(lv[0].asT<t_float>());
    }

    void m_minus(t_symbol* s, const AtomListView& lv)
    {
        if (!T::checkArgs(lv, T::ARG_FLOAT) || lv.size() != 1) {
            METHOD_ERR(s) << "single numeric argument expected: " << lv;
            return;
        }

        value() -= NumType(lv[0].asFloat());
    }

    void m_mul(t_symbol* s, const AtomListView& lv)
    {
        if (!T::checkArgs(lv, T::ARG_FLOAT) || lv.size() != 1) {
            METHOD_ERR(s) << "single numeric argument expected: " << lv;
            return;
        }

        value() *= NumType(lv[0].asFloat());
    }

    void m_div(t_symbol* s, const AtomListView& lv)
    {
        if (!T::checkArgs(lv, T::ARG_FLOAT) || lv.size() != 1) {
            METHOD_ERR(s) << "single numeric argument expected: " << lv;
            return;
        }

        t_float v = lv[0].asFloat();
        if (std::equal_to<t_float>()(v, 0)) {
            METHOD_ERR(s) << "division by zero";
            return;
        }

        value() /= NumType(v);
    }

    void m_set(t_symbol* /*s*/, const AtomListView& lv)
    {
        proto_set(lv);
    }

    virtual NumType& value() = 0;
    virtual const NumType& value() const = 0;

    void proto_set(const AtomListView& lv) override
    {
        static t_symbol* SYM_SET = gensym("set");

        if (!T::checkArgs(lv, T::ARG_FLOAT)) {
            METHOD_ERR(SYM_SET) << "numeric argument expected: " << lv;
            return;
        }

        value() = static_cast<NumType>(lv[0].asFloat());
    }
};

template <typename T>
class CollectionIFace : public BaseIFace<T> {
public:
    CollectionIFace(const PdArgs& args)
        : BaseIFace<T>(args)
    {
        T::createCbIntProperty("@size", [this]() { return proto_size(); })
            ->setIntCheck(PropValueConstraints::GREATER_EQUAL, 0);

        T::createCbBoolProperty("@empty", [this]() { return proto_size() == 0; });
    }

    virtual void proto_add(const AtomListView& lv) = 0;
    virtual bool proto_remove(const AtomListView& lv) = 0;
    virtual void proto_clear() = 0;
    virtual size_t proto_size() const = 0;
    virtual bool proto_choose(Atom& a) const = 0;

    void m_add(t_symbol* s, const AtomListView& lv)
    {
        if (lv.empty()) {
            METHOD_ERR(s) << "empty list";
            return;
        }

        proto_add(lv);
    }

    void m_remove(t_symbol* s, const AtomListView& lv)
    {
        if (!proto_remove(lv))
            METHOD_ERR(s) << "can't remove element at: " << lv;
    }

    void m_clear(t_symbol* /*s*/, const AtomListView& /*lv*/)
    {
        proto_clear();
    }

    void m_choose(t_symbol* /*s*/, const AtomListView& /*lv*/)
    {
        Atom a;
        if (proto_choose(a))
            this->atomTo(0, a);
    }
};

template <typename T>
class ListIFace : public BaseIFace<T> {
public:
    ListIFace(const PdArgs& args)
        : BaseIFace<T>(args)
    {
        T::createCbIntProperty("@size", [this]() { return proto_size(); })
            ->setIntCheck(PropValueConstraints::GREATER_EQUAL, 0);

        T::createCbBoolProperty("@empty", [this]() { return proto_size() == 0; });
    }

    // get methods
    virtual bool proto_at(int pos, Atom& a) const = 0;
    virtual bool proto_back(Atom& a) const = 0;
    virtual bool proto_front(Atom& a) const = 0;
    virtual bool proto_choose(Atom& a) const = 0;
    virtual size_t proto_size() const = 0;

    // change methods
    virtual void proto_append(const AtomListView& lv) = 0;
    virtual void proto_prepend(const AtomListView& lv) = 0;
    virtual bool proto_insert(size_t idx, const AtomListView& lv) = 0;
    virtual bool proto_pop() = 0;
    virtual bool proto_removeAt(size_t idx) = 0;
    virtual void proto_clear() = 0;
    virtual void proto_fill(const Atom& v) = 0;

    virtual void proto_sort() = 0;
    virtual void proto_reverse() = 0;
    virtual void proto_shuffle() = 0;

    void m_append(t_symbol* /*s*/, const AtomListView& lv)
    {
        proto_append(lv);
    }

    void m_prepend(t_symbol* /*s*/, const AtomListView& lv)
    {
        proto_prepend(lv);
    }

    void m_insert(t_symbol* s, const AtomListView& lv)
    {
        if (lv.size() < 2) {
            METHOD_ERR(s) << "POS ARG expected";
            return;
        }

        const size_t N = proto_size();
        int idx = lv[0].asInt();
        if (idx < 0)
            idx += N;

        if (idx > N || idx < 0) {
            METHOD_ERR(s) << "invalid position: " << lv[0];
            return;
        }

        if (!proto_insert(idx, lv.subView(1)))
            METHOD_ERR(s) << "can't insert to " << lv[0];
    }

    void m_pop(t_symbol* s, const AtomListView& /*lv*/)
    {
        if (proto_size() < 1) {
            METHOD_ERR(s) << "empty collection";
            return;
        }

        proto_pop();
    }

    void m_removeAt(t_symbol* s, const AtomListView& lv)
    {
        if (!T::checkArgs(lv, T::ARG_INT)) {
            METHOD_ERR(s) << "POS expected";
            return;
        }

        const int N = proto_size();
        int idx = lv[0].asInt();
        if (idx < 0)
            idx += N;

        if (idx >= N || idx < 0) {
            METHOD_ERR(s) << "invalid position: " << lv[0];
            return;
        }

        if (!proto_removeAt(idx))
            METHOD_ERR(s) << "can't remove element: " << lv[0];
    }

    void m_clear(t_symbol* /*s*/, const AtomListView& /*lv*/)
    {
        proto_clear();
    }

    void m_fill(t_symbol* s, const AtomListView& lv)
    {
        if (lv.empty()) {
            METHOD_ERR(s) << "fill value is required";
            return;
        }

        proto_fill(lv[0]);
    }

    void m_sort(t_symbol* /*s*/, const AtomListView& /*lv*/)
    {
        proto_sort();
    }

    void m_reverse(t_symbol* /*s*/, const AtomListView& /*lv*/)
    {
        proto_reverse();
    }

    void m_shuffle(t_symbol* /*s*/, const AtomListView& /*lv*/)
    {
        proto_shuffle();
    }

    void m_choose(t_symbol* /*s*/, const AtomListView& /*lv*/)
    {
        Atom a;
        if (proto_choose(a))
            this->atomTo(0, a);
    }

    void m_front(t_symbol* s, const AtomListView& /*lv*/)
    {
        Atom a;
        if (proto_front(a))
            this->atomTo(0, a);
        else
            METHOD_ERR(s) << "empty list";
    }

    void m_back(t_symbol* s, const AtomListView& /*lv*/)
    {
        Atom a;
        if (proto_back(a))
            this->atomTo(0, a);
        else
            METHOD_ERR(s) << "empty list";
    }

    void m_at(t_symbol* s, const AtomListView& lv)
    {
        const auto N = static_cast<int>(proto_size());
        const bool ok = lv.isFloat() && lv[0].isInteger() && (lv[0].asT<int>() >= (-N) && lv[0].asT<int>() < N);

        if (!ok) {
            METHOD_ERR(s) << "abs/relative index expected in [-" << N << "..+" << N << ") range";
            return;
        }

        Atom a;
        if (proto_at(lv[0].asT<int>(), a))
            this->atomTo(0, a);
    }
};

template <typename T>
class ReaderIFace : public T {
public:
    ReaderIFace(const PdArgs& args)
        : T(args)
    {
    }

    void m_read(t_symbol* s, const AtomListView& path)
    {
        std::string concat_path = to_string(path);
        if (concat_path.empty()) {
            METHOD_ERR(s) << "empty path";
            return;
        }

        auto full_path = platform::make_abs_filepath_with_canvas(T::canvas(), concat_path.c_str());
        if (full_path.empty()) {
            full_path = platform::find_in_std_path(T::canvas(), concat_path.c_str());
            if (full_path.empty()) {
                METHOD_ERR(s) << "invalid path: " << path;
                return;
            }
        }

        if (!proto_read(full_path)) {
            METHOD_ERR(s) << "can not read from file: " << full_path;
            return;
        }
    }

    virtual bool proto_read(const std::string& path) = 0;
};

template <typename T>
class WriterIFace : public T {
public:
    WriterIFace(const PdArgs& args)
        : T(args)
    {
    }

    void m_write(t_symbol* s, const AtomListView& path)
    {
        std::string concat_path = to_string(path);
        if (concat_path.empty()) {
            METHOD_ERR(s) << "empty path";
            return;
        }

        auto full_path = platform::make_abs_filepath_with_canvas(T::canvas(), concat_path);
        if (full_path.empty()) {
            METHOD_ERR(s) << "invalid path: " << path;
            return;
        }

        // warning
        if (platform::path_exists(full_path.c_str()))
            METHOD_DBG(s) << "overwriting file that already exists: " << full_path;

        if (!proto_write(full_path))
            METHOD_ERR(s) << "can not write to JSON: " << full_path;
    }

    virtual bool proto_write(const std::string& path) const = 0;
};

template <typename T>
class FilesystemIFace : public WriterIFace<ReaderIFace<T>> {
public:
    FilesystemIFace(const PdArgs& args)
        : WriterIFace<ReaderIFace<T>>(args)
    {
    }
};

namespace protocol {
    template <template <typename T> class Factory, typename T>
    class Base {
    public:
        Base(Factory<T>& obj)
        {
            obj.addMethod("set", &T::m_set);
        }
    };

    template <template <typename T> class Factory, typename T>
    class Numeric : public Base<Factory, T> {
    public:
        Numeric(Factory<T>& obj)
            : Base<Factory, T>(obj)
        {
            obj.addMethod("+", &T::m_plus);
            obj.addMethod("-", &T::m_minus);
            obj.addMethod("*", &T::m_mul);
            obj.addMethod("/", &T::m_div);
        }
    };

    template <template <typename T> class Factory, typename T>
    class Collection : public Base<Factory, T> {
    public:
        Collection(Factory<T>& obj)
            : Base<Factory, T>(obj)
        {
            // void proto_add(const AtomList&)
            obj.addMethod("add", &T::m_add);

            // bool proto_remove(const AtomList&)
            obj.addMethod("remove", &T::m_remove);

            // void proto_clear()
            obj.addMethod("clear", &T::m_clear);

            // void proto_choose(Atom&)
            obj.addMethod("choose", &T::m_choose);
        }
    };

    template <template <typename T> class Factory, typename T>
    class SeqCollection : public Base<Factory, T> {
    public:
        SeqCollection(Factory<T>& obj)
            : Base<Factory, T>(obj)
        {
            // void proto_append(const AtomList& lv)
            obj.addMethod("append", &T::m_append);

            // void proto_append(const AtomList& lv)
            obj.addMethod("prepend", &T::m_prepend);

            // bool proto_insert(int idx, const AtomList& lv)
            obj.addMethod("insert", &T::m_insert);

            // bool proto_pop()
            obj.addMethod("pop", &T::m_pop);

            // bool proto_remove_at(int idx)
            obj.addMethod("remove", &T::m_removeAt);

            // void proto_clear()
            obj.addMethod("clear", &T::m_clear);

            // void proto_fill(const Atom& v)
            obj.addMethod("fill", &T::m_fill);

            // void proto_reverse()
            obj.addMethod("reverse", &T::m_reverse);

            // void proto_sort()
            obj.addMethod("sort", &T::m_sort);

            // void proto_shuffle()
            obj.addMethod("shuffle", &T::m_shuffle);

            // void proto_choose()
            obj.addMethod("choose", &T::m_choose);

            // bool proto_front(Atom&)
            obj.addMethod("front", &T::m_front);

            // bool proto_back(Atom&)
            obj.addMethod("back", &T::m_back);

            // bool proto_at(int pos, Atom&)
            obj.addMethod("at", &T::m_at);
        }
    };

    template <template <typename T> class Factory, typename T>
    class Reader : public Base<Factory, T> {
    public:
        Reader(Factory<T>& obj)
            : Base<Factory, T>(obj)
        {
            obj.addMethod("read", &T::m_read);
        }
    };

    template <template <typename T> class Factory, typename T>
    class Writer : public Base<Factory, T> {
    public:
        Writer(Factory<T>& obj)
            : Base<Factory, T>(obj)
        {
            obj.addMethod("write", &T::m_write);
        }
    };

    template <template <typename T> class Factory, typename T>
    class Storage {
    public:
        Storage(Factory<T>* obj)
        {
            obj->addMethod("clear", &T::m_clear);
            obj->addMethod("store", &T::m_store);
            obj->addMethod("load", &T::m_load);
            obj->addMethod("update", &T::m_update);
            obj->addMethod("interp", &T::m_interp);
        }
    };
}

template <typename T>
class BaseIFaceFactory : public ObjectFactory<T> {
public:
    BaseIFaceFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
        : ObjectFactory<T>(name, flags)
    {
        protocol::Base<ObjectFactory, T> proto(*this);
    }
};

template <typename T>
class NumericIFaceFactory : public ObjectFactory<T> {
public:
    NumericIFaceFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
        : ObjectFactory<T>(name, flags)
    {
        protocol::Numeric<ObjectFactory, T> proto(*this);
    }
};

template <typename T>
class ColectionIFaceFactory : public ObjectFactory<T> {
public:
    ColectionIFaceFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
        : ObjectFactory<T>(name, flags)
    {
        protocol::Collection<ObjectFactory, T> proto(*this);
    }
};

template <typename T>
class ListIFaceFactory : public ObjectFactory<T> {
public:
    ListIFaceFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
        : ObjectFactory<T>(name, flags)
    {
        protocol::SeqCollection<ObjectFactory, T> proto(*this);
    }
};
}

#endif // DATA_PROTOCOL_H
