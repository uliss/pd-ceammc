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

#include "ceammc_args.h"
#include "ceammc_data.h"
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
        T::createCbListProperty("@keys", [this]() -> AtomList { return dict()->keys(); });
    }

    void dump() const override
    {
        FilesystemIFace<CollectionIFace<T>>::dump();
        OBJ_DBG << dict();
    }

    void onBang() override
    {
        this->atomTo(0, dict());
    }

    void onList(const AtomList& args) override
    {
        if (args.size() % 2 != 0) {
            OBJ_ERR << "list of key-value pairs expected: " << args;
            return;
        }

        dict().detachData();
        *dict() = DataTypeDict::fromList(args, 2);
    }

    void onAny(t_symbol* s, const AtomListView& args) override
    {
        std::string str(s->s_name);
        str += ' ';
        str += to_string(args, " ");

        dict().detachData();

        if (!dict()->fromString(str)) {
            OBJ_ERR << "parse error: " << s << args;
            return;
        }
    }

    void onDataT(const DictAtom& d)
    {
        dict() = d;
        onBang();
    }

    void m_get_key(t_symbol* s, const AtomListView& key)
    {
        if (!key.isSymbol()) {
            METHOD_ERR(s) << "key expected";
            METHOD_ERR(s) << "usage: " << s << " KEY";
            return;
        }

        auto k = key.asT<t_symbol*>();

        if (dict()->contains(k))
            this->listTo(0, dict()->at(k));
    }

    void m_set_key(t_symbol* s, const AtomListView& lst)
    {
        if (lst.empty() || !lst[0].isSymbol()) {
            METHOD_ERR(s) << "key expected";
            METHOD_ERR(s) << "usage: " << s << " KEY [VALUES...]";
            return;
        }

        auto key = lst[0].asSymbol();

        if (!dict()->contains(key)) {
            METHOD_ERR(s) << "key not found: " << lst[0];
            return;
        }

        dict().detachData();
        dict()->at(key) = lst.subView(1);
    }

    void proto_add(const AtomListView& lst) override
    {
        if (lst.empty() || !lst[0].isSymbol()) {
            OBJ_ERR << "Usage: add KEY [VALUES...]";
            return;
        }

        dict().detachData();
        auto key = lst[0].asSymbol();

        if (lst.size() == 2)
            dict()->insert(key, lst[1]);
        else
            dict()->insert(key, lst.subView(1));
    }

    void proto_clear() override
    {
        dict().detachData();
        dict()->clear();
    }

    void proto_set(const AtomListView& lst) override
    {
        dict().detachData();
        dict()->fromString(to_string(lst, " "));
    }

    bool proto_remove(const AtomListView& lst) override
    {
        if (lst.empty() || !lst.anyOf(isSymbol)) {
            OBJ_ERR << "key list expected, got: " << lst;
            return false;
        }

        bool res = true;
        dict().detachData();

        for (auto& el : lst) {
            if (!dict()->remove(el.asSymbol())) {
                res = false;
                OBJ_ERR << "key not found: " << el;
            }
        }

        return res;
    }

    size_t proto_size() const override
    {
        return dict()->size();
    }

    bool proto_write(const std::string& path) const override
    {
        return dict()->write(path);
    }

    bool proto_read(const std::string& path) override
    {
        dict().detachData();
        return dict()->read(path);
    }

    virtual const DictAtom& dict() const = 0;
    virtual DictAtom& dict() = 0;
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
