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
#ifndef DICT_IFACE_H
#define DICT_IFACE_H

#include "ceammc_object.h"
#include "data_protocol.h"
#include "datatype_dict.h"

using namespace ceammc;

template <typename T>
class DictIFace : public FilesystemIFace<CollectionIFace<T>> {
public:
    DictIFace(const PdArgs& args)
        : FilesystemIFace<CollectionIFace<T>>(args)
    {
        T::createCbProperty("@keys", &DictIFace::propKeys);
    }

    void dump() const override
    {
        FilesystemIFace<CollectionIFace<T>>::dump();
        OBJ_DBG << dict().toString();
    }

    void onBang() override
    {
        this->dataTo(0, dict().clone());
    }

    void onList(const AtomList& args) override
    {
        if (args.size() % 2 != 0) {
            OBJ_ERR << "list of key-value pairs expected: " << args;
            return;
        }

        dict().clear();

        for (size_t i = 0; i < args.size(); i += 2)
            dict().insert(args[i], args[i + 1]);
    }

    void onAny(t_symbol* s, const AtomList& args) override
    {
        std::string str(s->s_name);
        str += ' ';
        str += to_string(args, " ");

        if (!dict().fromString(str)) {
            OBJ_ERR << "parse error: " << s << args;
            return;
        }
    }

    void onDataT(const DataTPtr<DataTypeDict>& d)
    {
        dict() = *d.data();
        onBang();
    }

    void m_get_key(t_symbol* s, const AtomList& key)
    {
        if (key.empty()) {
            METHOD_ERR(s) << "key expected";
            METHOD_ERR(s) << "usage: " << s << " KEY";
            return;
        }

        auto v = dict().value(key[0]);
        if (DataTypeDict::isNull(v))
            return;

        if (v.type() == typeid(Atom))
            this->atomTo(0, boost::get<Atom>(v));
        else if (v.type() == typeid(AtomList))
            this->listTo(0, boost::get<AtomList>(v));
        else if (v.type() == typeid(DataAtom))
            this->atomTo(0, boost::get<DataAtom>(v).toAtom());
        else
            METHOD_ERR(s) << "unknown value type";
    }

    void m_set_key(t_symbol* s, const AtomList& lst)
    {
        if (lst.empty()) {
            METHOD_ERR(s) << "key expected";
            METHOD_ERR(s) << "usage: " << s << " KEY VALUES...";
            return;
        }

        if (lst.size() < 2) {
            METHOD_ERR(s) << "value expected";
            METHOD_ERR(s) << "usage: " << s << " KEY VALUES...";
            return;
        }

        auto key = lst[0];

        if (!dict().contains(key)) {
            METHOD_ERR(s) << "key not found: " << key;
            return;
        }

        proto_add(lst);
    }

    void proto_add(const AtomList& lst) override
    {
        if (lst.size() < 2) {
            OBJ_ERR << "Usage: add KEY VALUES...";
            return;
        }

        if (lst.size() == 2) {
            if (lst[1].isData())
                dict().insert(lst[0], DataAtom(lst[1]));
            else
                dict().insert(lst[0], lst[1]);
        } else
            dict().insert(lst[0], lst.slice(1));
    }

    void proto_clear() override
    {
        dict().clear();
    }

    void proto_set(const AtomList& lst) override
    {
        dict().fromString(to_string(lst, " "));
    }

    bool proto_remove(const AtomList& lst) override
    {
        bool res = true;

        for (auto& el : lst) {
            if (!dict().remove(el)) {
                res = false;
                OBJ_ERR << "key not found: " << el;
            }
        }

        return res;
    }

    size_t proto_size() const override
    {
        return dict().size();
    }

    bool proto_write(const std::string& path) const override
    {
        return dict().write(path);
    }

    bool proto_read(const std::string& path) override
    {
        return dict().read(path);
    }

    AtomList propKeys() const
    {
        AtomList res;

        res.reserve(dict().size());
        for (auto& el : dict().innerData())
            res.append(el.first);

        return res;
    }

public:
    virtual DataTypeDict& dict() = 0;
    virtual const DataTypeDict& dict() const = 0;
};

template <typename T>
class DictIFaceFactory : public ColectionIFaceFactory<T> {
public:
    DictIFaceFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
        : ColectionIFaceFactory<T>(name, flags)
    {
        protocol::Reader<ObjectFactory, T> reader(*this);
        protocol::Writer<ObjectFactory, T> writer(*this);

        this->addMethod("get_key", &T::m_get_key);
        this->addMethod("set_key", &T::m_set_key);

        this->template processData<DataTypeDict>();
    }
};

#endif // DICT_IFACE_H
