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
#include "dict_get.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "fmt/core.h"

bool DictGetKeyData::isMatch() const
{
    return key_ == &s_ && match_.size() > 0;
}

constexpr size_t MAX_KEYS = 32;

DictGet::DictGet(const PdArgs& args)
    : DictBase(args)
    , default_(nullptr)
{
    keys_ = new ListProperty("@keys");
    keys_->setInitOnly();
    keys_->setArgIndex(0);
    keys_->setListCheckFn([this](const AtomListView& lv) {
        if (!lv.anyOf(isSymbol)) {
            OBJ_ERR << "only symbols are allowed as key, got: " << lv;
            return false;
        }

        if (lv.size() > MAX_KEYS) {
            OBJ_ERR << "only " << MAX_KEYS << " max keys are allowed, got: " << lv.size();
            return false;
        }

        return true;
    });
    addProperty(keys_);

    default_ = new AtomProperty("@default", Atom());
    addProperty(default_);
}

void DictGet::initDone()
{
    for (auto& a : keys_->value()) {
        auto key = a.asSymbol()->s_name;

        if (key[0] == '/') {
            parser::DictExprMatchList m;
            if (parser::parse_dict_match_expr(key, &m)) {
                key_data_.push_back({ &s_, m });
                createOutlet();
            } else {
                OBJ_ERR << fmt::format("can't parse dict expr path: {}", key);
                continue;
            }
        } else {
            key_data_.push_back({ a.asSymbol(), {} });
            createOutlet();
        }
    }
}

void DictGet::onDataT(const DictAtom& dict)
{
    long n = key_data_.size();

    // back order
    while (n-- > 0) {
        auto& k = key_data_[n];
        if (!k.isMatch()) {
            if (dict->contains(k.key_))
                listTo(n, dict->at(k.key_));
            else if (!default_->value().isNone())
                atomTo(n, default_->value());
            else
                ; // no output
        } else {
            findMatches(dict, k.match_.data(), k.match_.size(), n);
        }
    }
}

void DictGet::findMatches(const AtomListView& lv, const parser::DictExprMatcher* m, size_t level, size_t outlet)
{
    if (!m || !level)
        return;

    if (lv.isA<DataTypeDict>()) {
        for (auto& kv : *lv.asD<DataTypeDict>()) {
            switch (m->type) {
            case parser::DictExprMatchType::ANY: {
                if (level == 1) // last level
                    listTo(outlet, kv.second);
                else
                    findMatches(kv.second, m + 1, level - 1, outlet);
            } break;
            case parser::DictExprMatchType::DICT: {
                if (m->key_name == kv.first) {
                    if (level == 1)
                        listTo(outlet, kv.second);
                    else
                        findMatches(kv.second, m + 1, level - 1, outlet);
                }
            } break;
            case parser::DictExprMatchType::LIST: {
                if (!m->no_error)
                    OBJ_ERR << fmt::format("can't index dict with index: '{}'", m->array_slice_begin);
            } break;
            default:
                break;
            }
        }
    } else if (lv.isA<DataTypeMList>()) {
        findMatches(lv.asD<DataTypeMList>()->data().view(), m, level, outlet);
    } else {
        switch (m->type) {
        case parser::DictExprMatchType::LIST: {
            auto pos = m->arraySlice(lv.size());

            if (pos.second > 0) {
                if (level == 1)
                    listTo(outlet, lv.subView(pos.first, pos.second));
                else
                    findMatches(lv.subView(pos.first, pos.second), m + 1, level - 1, outlet);
            }
        } break;
        case parser::DictExprMatchType::ANY:
            if (level == 1) {
                for (auto& a : lv)
                    atomTo(outlet, a);
            } else {
                for (auto& a : lv)
                    findMatches(a, m + 1, level - 1, outlet);
            }
            break;
        default:
            if (!m->no_error)
                OBJ_ERR << fmt::format("can't index list with key: '{}'", m->key_name->s_name);
            break;
        }
    }
}

void setup_dict_get()
{
    ObjectFactory<DictGet> obj("dict.get");
    obj.processData<DataTypeDict>();

    obj.setDescription("output dict values");
    obj.setCategory("data");
    obj.setKeywords({ "get", "dictionary" });
}
