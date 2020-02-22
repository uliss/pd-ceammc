/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "datatype_tree.h"
#include "ceammc_datatypes.h"
#include "datatype_tree_imp.h"
#include "fmt/format.h"

namespace ceammc {

const DataType DataTypeTree::dataType = data::DATA_TREE;

DataTypeTree::DataTypeTree(DataTypeTreeImpl* pimpl)
    : pimpl_(pimpl)
    , copy_on_write_(false)
{
}

DataTypeTree::DataTypeTree()
    : pimpl_(nullInstance().pimpl_)
    , copy_on_write_(true)
{
}

DataTypeTree::~DataTypeTree()
{
}

DataTypeTree::DataTypeTree(const DataTypeTree& tree)
    : pimpl_(tree.pimpl_)
    , copy_on_write_(true)
{
}

DataTypeTree::DataTypeTree(DataTypeTree&& tree)
    : pimpl_(tree.pimpl_)
    , copy_on_write_(false)
{
    tree.pimpl_ = nullInstance().pimpl_;
    tree.copy_on_write_ = true;
}

DataTypeTree::DataTypeTree(const DataTypeTreeImpl& imp)
    : pimpl_(new DataTypeTreeImpl(imp))
    , copy_on_write_(false)
{
}

DataTypeTree::DataTypeTree(DataTypeTreeImpl&& imp)
    : pimpl_(new DataTypeTreeImpl(std::move(imp)))
    , copy_on_write_(false)
{
}

DataTypeTree::DataTypeTree(t_float f)
    : pimpl_(new DataTypeTreeImpl(f))
    , copy_on_write_(false)
{
}

DataTypeTree::DataTypeTree(t_symbol* s)
    : pimpl_(new DataTypeTreeImpl(s))
    , copy_on_write_(false)
{
}

DataTypeTree::DataTypeTree(const char* s)
    : pimpl_(new DataTypeTreeImpl(s))
    , copy_on_write_(false)
{
}

DataTypeTree::DataTypeTree(const std::string& s)
    : pimpl_(new DataTypeTreeImpl(s.c_str()))
    , copy_on_write_(false)
{
}

DataTypeTree::DataTypeTree(const FloatList& lst)
    : pimpl_(new DataTypeTreeImpl(lst))
    , copy_on_write_(false)
{
}

DataTypeTree::DataTypeTree(const AtomList& lst)
    : pimpl_(new DataTypeTreeImpl(lst))
    , copy_on_write_(false)
{
}

DataTypeTree& DataTypeTree::operator=(const DataTypeTree& t)
{
    if (this != &t) {
        pimpl_ = t.pimpl_;
        copy_on_write_ = true;
    }

    return *this;
}

DataTypeTree* DataTypeTree::clone() const
{
    return new DataTypeTree(*this);
}

DataType DataTypeTree::type() const
{
    return data::DATA_TREE;
}

std::string DataTypeTree::toString() const
{
    return DataTypeTreeImpl::jsonToTree(pimpl_->toString());
}

bool DataTypeTree::isEqual(const AbstractData* d) const
{
    if (!d)
        return false;

    const DataTypeTree* data = d->as<DataTypeTree>();

    // not a dict
    if (!data)
        return false;

    // self check
    if (data == this)
        return true;

    return *data == *this;
}

bool DataTypeTree::operator==(const DataTypeTree& d) const
{
    if (this == &d)
        return true;
    else if (pimpl_ == d.pimpl_)
        return true;
    else
        return *pimpl_ == *d.pimpl_;
}

size_t DataTypeTree::size() const
{
    return pimpl_->size();
}

bool DataTypeTree::empty() const
{
    return pimpl_->empty();
}

bool DataTypeTree::isArray() const
{
    return pimpl_->isArray();
}

bool DataTypeTree::isSimpleArray() const
{
    return pimpl_->isSimpleArray();
}

bool DataTypeTree::isNull() const
{
    return pimpl_->isNull();
}

bool DataTypeTree::isFloat() const
{
    return pimpl_->isFloat();
}

bool DataTypeTree::isString() const
{
    return pimpl_->isString();
}

bool DataTypeTree::isObject() const
{
    return pimpl_->isObject();
}

EitherTreeFloat DataTypeTree::getFloat() const
{
    return pimpl_->getFloat();
}

EitherTreeSymbol DataTypeTree::getSymbol() const
{
    using F = std::function<t_symbol*(const std::string&)>;
    F fn = [](const std::string& s) { return gensym(s.c_str()); };
    return pimpl_->getString().call(fn);
}

EitherTree DataTypeTree::getList() const
{
    if (isArray())
        return *this;
    else
        return EitherTree::makeError(fmt::format("array expected instead of {0}", pimpl_->typeName()));
}

EitherTree DataTypeTree::getObject() const
{
    if (isObject())
        return *this;
    else
        return EitherTree::makeError(fmt::format("object expected instead of {0}", pimpl_->typeName()));
}

AtomList DataTypeTree::keys() const
{
    AtomList keys;
    auto pkeys = pimpl_->keys();
    keys.reserve(pkeys.size());

    for (auto& k : pkeys)
        keys.append(gensym(k.c_str()));

    return keys;
}

void DataTypeTree::clear()
{
    if (!is_null()) {
        detach_pimpl();
        pimpl_->clear();
    }
}

bool DataTypeTree::addFloat(t_float f)
{
    detach_pimpl();
    return pimpl_->addFloat(f);
}

bool DataTypeTree::addSymbol(t_symbol* s)
{
    detach_pimpl();
    return pimpl_->addSymbol(s);
}

bool DataTypeTree::addList(const AtomList& l)
{
    detach_pimpl();
    return pimpl_->addList(l);
}

bool DataTypeTree::addTree(const DataTypeTree& tree)
{
    detach_pimpl();
    return pimpl_->addTree(*tree.pimpl_);
}

void DataTypeTree::setFloat(t_float f)
{
    detach_pimpl();
    pimpl_->setFloat(f);
}

void DataTypeTree::setSymbol(t_symbol* s)
{
    detach_pimpl();
    pimpl_->setSymbol(s);
}

bool DataTypeTree::set(const DataTPtr<DataTypeTree>& ptr)
{
    if (ptr.isValid()) {
        detach_pimpl();
        *this = *ptr.data();
        return true;
    } else
        return false;
}

DataTPtr<DataTypeTree> DataTypeTree::match(const char* pattern) const
{
    return new DataTypeTree(pimpl_->match(pattern));
}

DataTypeTree DataTypeTree::at(size_t idx) const
{
    return pimpl_->at(idx);
}

DataTypeTree DataTypeTree::at(t_symbol* key) const
{
    return pimpl_->at(key->s_name);
}

bool DataTypeTree::insertFloat(const char* key, t_float f)
{
    detach_pimpl();
    return pimpl_->insertFloat(key, f);
}

bool DataTypeTree::insertSymbol(const char* key, t_symbol* s)
{
    detach_pimpl();
    return pimpl_->insertSymbol(key, s);
}

bool DataTypeTree::insertTree(const char* key, const DataTypeTree& tree)
{
    detach_pimpl();
    return pimpl_->insertTree(key, *tree.pimpl_);
}

void DataTypeTree::outputTo(_outlet* o) const
{
    if (isNull())
        return;
    else if (isFloat())
        outlet_float(o, pimpl_->asFloat());
    else if (isString())
        outlet_symbol(o, pimpl_->asSymbol());
    else {
        if (pimpl_->isSimpleArray()) {
            auto lst = pimpl_->asAtomList();
            outlet_list(o, &s_list, lst.size(), lst.toPdData());
        } else {
            DataPtr dptr(clone());
            dptr.asAtom().output(o);
        }
    }
}

Atom DataTypeTree::asAtom() const
{
    if (isFloat())
        return pimpl_->asFloat();
    else if (isString())
        return pimpl_->asSymbol();
    else
        return Atom();
}

AtomList DataTypeTree::asAtomList() const
{
    return pimpl_->asAtomList();
}

DataTPtr<DataTypeTree> DataTypeTree::asDataPtr() const
{
    return DataTPtr<DataTypeTree>(clone());
}

bool DataTypeTree::parse(const char* str)
{
    detach_pimpl();
    return pimpl_->parse(str);
}

DataTPtr<DataTypeTree> DataTypeTree::newFromString(const char* str)
{
    DataTypeTree* p = new DataTypeTree;
    p->parse(str);
    return p;
}

DataTPtr<DataTypeTree> DataTypeTree::newFromString(const std::string& str)
{
    return newFromString(str.c_str());
}

DataTPtr<DataTypeTree> DataTypeTree::newEmpty()
{
    return new DataTypeTree;
}

DataTypeTree DataTypeTree::fromString(const std::string& str)
{
    return fromString(str.c_str());
}

DataTypeTree DataTypeTree::fromString(const char* str)
{
    return { DataTypeTreeImpl::fromString(str) };
}

void DataTypeTree::detach_pimpl()
{
    if (copy_on_write_) {
        pimpl_.reset(new DataTypeTreeImpl(*pimpl_));
        copy_on_write_ = false;
    }
}

bool DataTypeTree::is_null() const
{
    return pimpl_ == nullInstance().pimpl_;
}

DataTypeTree& DataTypeTree::nullInstance()
{
    static DataTypeTree instance_(new DataTypeTreeImpl);
    return instance_;
}

std::ostream& operator<<(std::ostream& os, const DataTypeTree& t)
{
    os << t.toString();
    return os;
}

}
