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
#ifndef GLOBAL_BASE_H
#define GLOBAL_BASE_H

#include "ceammc_globaldata.h"
#include "ceammc_object.h"

#include <cstring>

namespace ceammc {

constexpr const char* DEFAULT_ID = "default";

template <typename T, typename Base = BaseObject>
class GlobalBase : public Base {
    std::unique_ptr<GlobalData<T>> data_;
    SymbolProperty* id_ { nullptr };
    mutable std::uint8_t call_count_ { 0 };
    GlobalBase(const GlobalBase&) = delete;
    void operator=(const GlobalBase&) = delete;

public:
    GlobalBase(const PdArgs& a)
        : Base(a)
        , data_(new GlobalData<T>(this->parsedPosArgs().symbolAt(0, gensym(DEFAULT_ID))))
    {
        id_ = new SymbolProperty("@id", gensym(DEFAULT_ID));
        id_->setInitOnly();
        id_->setArgIndex(0);
        this->addProperty(id_);
    }

    void initDone() override
    {
        Base::initDone();

        if (id_->value() != data_->name())
            data_.reset(new GlobalData<T>(id_->value()));

        // bind to global listener
        if (id_->value() != id_->info().defaultSymbol())
            this->bindReceive(id_->value());
    }

    void dump() const override
    {
        Base::dump();
        OBJ_POST << "ref count: " << refCount();

        // print keys
        {
            Post pk(this);
            pk.stream() << "existing keys:";
            std::vector<t_symbol*> keys;
            data_->keys(keys);
            for (auto k : keys)
                pk.stream() << ' ' << k;
        }
    }

    T& ref()
    {
        if (call_count_++ == 1 && data_->name() == id_->info().defaultSymbol()) {
            OBJ_DBG << "global object ID is required! Using global object with default id";
        }

        return data_->ref();
    }

    const T& ref() const
    {
        if (call_count_++ == 1 && data_->name() == id_->info().defaultSymbol()) {
            OBJ_DBG << "global object ID is required! Using global object with default id";
        }

        return data_->ref();
    }

    t_symbol* id() const { return data_->name(); }

    size_t refCount() const { return data_->refCount(); }

    virtual void outputRef(const T&) { }

    void m_get(t_symbol* s, const AtomListView& lv)
    {
        if (lv.empty()) {
            this->outputRef(ref());
            return;
        }

        if (!lv.isSymbol()) {
            METHOD_ERR(s) << "variable name expected, got: " << lv;
            return;
        }

        auto key = lv.asSymbol();
        if (GlobalData<T>::contains(key)) {
            auto data = GlobalData<T>(key);
            this->outputRef(data.ref());
        } else {
            OBJ_ERR << "variable is not found: " << key;
        }
    }

    template <typename Factory>
    static void globalBaseFactory(Factory& f)
    {
        f.addMethod("get", &GlobalBase<T, Base>::m_get);
    }
};
}

#endif // GLOBAL_BASE_H
