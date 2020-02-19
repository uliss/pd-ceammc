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

namespace ceammc {

const DataType DataTypeTree::dataType = data::DATA_TREE;

DataTypeTree::DataTypeTree()
    : pimpl_(new DataTypeTreeImpl)
{
}

DataTypeTree::~DataTypeTree()
{
    delete pimpl_;
}

DataTypeTree::DataTypeTree(const DataTypeTree& tree)
    : pimpl_(new DataTypeTreeImpl(*tree.pimpl_))
{
}

DataTypeTree::DataTypeTree(DataTypeTree&& tree)
    : pimpl_(tree.pimpl_)
{
    tree.pimpl_ = nullptr;
}

DataTypeTree::DataTypeTree(const DataTypeTreeImpl& imp)
    : pimpl_(new DataTypeTreeImpl(imp))
{
}

DataTypeTree::DataTypeTree(t_float f)
    : pimpl_(new DataTypeTreeImpl(f))
{
}

DataTypeTree::DataTypeTree(t_symbol* s)
    : pimpl_(new DataTypeTreeImpl(s))
{
}

DataTypeTree::DataTypeTree(const FloatList& lst)
    : pimpl_(new DataTypeTreeImpl(lst))
{
}

DataTypeTree::DataTypeTree(const char* str)
    : pimpl_(new DataTypeTreeImpl(str))
{
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
    return pimpl_->toString();
}

bool DataTypeTree::isEqual(const AbstractData* d) const
{
    auto data = d->as<DataTypeTree>();

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

bool DataTypeTree::isNull() const
{
    return pimpl_->isNull();
}

void DataTypeTree::clear()
{
    pimpl_->clear();
}

bool DataTypeTree::addFloat(t_float f)
{
    return pimpl_->addFloat(f);
}

bool DataTypeTree::addSymbol(t_symbol* s)
{
    return pimpl_->addSymbol(s);
}

bool DataTypeTree::addList(const AtomList& l)
{
    return pimpl_->addList(l);
}

bool DataTypeTree::addTree(const DataTypeTree& tree)
{
    return pimpl_->addTree(*tree.pimpl_);
}

void DataTypeTree::setFloat(t_float f)
{
    pimpl_->setFloat(f);
}

void DataTypeTree::setSymbol(t_symbol* s)
{
    pimpl_->setSymbol(s);
}

bool DataTypeTree::set(const DataTPtr<DataTypeTree>& ptr)
{
    if (ptr.isValid()) {
        pimpl_->set(*ptr.data()->pimpl_);
        return true;
    } else
        return false;
}

DataTPtr<DataTypeTree> DataTypeTree::match(const char* pattern) const
{
    return new DataTypeTree(pimpl_->match(pattern));
}

DataTPtr<DataTypeTree> DataTypeTree::at(size_t idx) const
{
    return new DataTypeTree(pimpl_->at(idx));
}

DataTPtr<DataTypeTree> DataTypeTree::at(t_symbol* key) const
{
    return new DataTypeTree(pimpl_->at(key->s_name));
}

bool DataTypeTree::insertFloat(const char* key, t_float f)
{
    return pimpl_->insertFloat(key, f);
}

bool DataTypeTree::insertSymbol(const char* key, t_symbol* s)
{
    return pimpl_->insertSymbol(key, s);
}

bool DataTypeTree::insertTree(const char* key, const DataTypeTree& tree)
{
    return pimpl_->insertTree(key, *tree.pimpl_);
}

bool DataTypeTree::parse(const char* str)
{
    return pimpl_->parse(str);
}

}
