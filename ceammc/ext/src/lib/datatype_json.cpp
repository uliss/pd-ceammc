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
#include "datatype_json.h"
#include "ceammc_datatypes.h"
#include "datatype_json_imp.h"

namespace ceammc {

const DataType DataTypeJson::dataType = data::DATA_TREE;

DataTypeJson::DataTypeJson()
    : pimpl_(new DataTypeJsonImpl)
{
}

DataTypeJson::~DataTypeJson()
{
    delete pimpl_;
}

DataTypeJson::DataTypeJson(const DataTypeJson& json)
    : pimpl_(new DataTypeJsonImpl(*json.pimpl_))
{
}

DataTypeJson::DataTypeJson(DataTypeJson&& json)
    : pimpl_(json.pimpl_)
{
    json.pimpl_ = nullptr;
}

DataTypeJson::DataTypeJson(const DataTypeJsonImpl& imp)
    : pimpl_(new DataTypeJsonImpl(imp))
{
}

DataTypeJson::DataTypeJson(t_float f)
    : pimpl_(new DataTypeJsonImpl(f))
{
}

DataTypeJson::DataTypeJson(t_symbol* s)
    : pimpl_(new DataTypeJsonImpl(s))
{
}

DataTypeJson::DataTypeJson(const FloatList& lst)
    : pimpl_(new DataTypeJsonImpl(lst))
{
}

DataTypeJson::DataTypeJson(const char* str)
    : pimpl_(new DataTypeJsonImpl(str))
{
}

DataTypeJson* DataTypeJson::clone() const
{
    return new DataTypeJson(*this);
}

DataType DataTypeJson::type() const
{
    return data::DATA_TREE;
}

std::string DataTypeJson::toString() const
{
    return pimpl_->toString();
}

bool DataTypeJson::isEqual(const AbstractData* d) const
{
    auto data = d->as<DataTypeJson>();

    // not a dict
    if (!data)
        return false;

    // self check
    if (data == this)
        return true;

    return *data == *this;
}

bool DataTypeJson::operator==(const DataTypeJson& d) const
{
    return *pimpl_ == *d.pimpl_;
}

size_t DataTypeJson::size() const
{
    return pimpl_->size();
}

bool DataTypeJson::empty() const
{
    return pimpl_->empty();
}

bool DataTypeJson::isArray() const
{
    return pimpl_->isArray();
}

bool DataTypeJson::isNull() const
{
    return pimpl_->isNull();
}

JsonValue DataTypeJson::valueAt(size_t idx) const
{
    return pimpl_->valueAt(idx);
}

void DataTypeJson::clear()
{
    pimpl_->clear();
}

bool DataTypeJson::addFloat(t_float f)
{
    return pimpl_->addFloat(f);
}

bool DataTypeJson::addSymbol(t_symbol* s)
{
    return pimpl_->addSymbol(s);
}

bool DataTypeJson::addList(const AtomList& l)
{
    return pimpl_->addList(l);
}

bool DataTypeJson::addJson(const DataTypeJson& json)
{
    return pimpl_->addJson(*json.pimpl_);
}

void DataTypeJson::setFloat(t_float f)
{
    pimpl_->setFloat(f);
}

void DataTypeJson::setSymbol(t_symbol* s)
{
    pimpl_->setSymbol(s);
}

bool DataTypeJson::set(const DataTPtr<DataTypeJson>& ptr)
{
    if (ptr.isValid()) {
        pimpl_->set(*ptr.data()->pimpl_);
        return true;
    } else
        return false;
}

DataTPtr<DataTypeJson> DataTypeJson::match(const char* pattern) const
{
    return new DataTypeJson(pimpl_->match(pattern));
}

DataTPtr<DataTypeJson> DataTypeJson::at(size_t idx) const
{
    return new DataTypeJson(pimpl_->at(idx));
}

DataTPtr<DataTypeJson> DataTypeJson::at(t_symbol* key) const
{
    return new DataTypeJson(pimpl_->at(key->s_name));
}

bool DataTypeJson::insertFloat(const char* key, t_float f)
{
    return pimpl_->insertFloat(key, f);
}

bool DataTypeJson::insertSymbol(const char* key, t_symbol* s)
{
    return pimpl_->insertSymbol(key, s);
}

bool DataTypeJson::insertJson(const char* key, const DataTypeJson& json)
{
    return pimpl_->insertJson(key, *json.pimpl_);
}

bool DataTypeJson::parse(const char* str)
{
    return pimpl_->parse(str);
}

}
