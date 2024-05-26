/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "route_data.h"
#include "ceammc_datastorage.h"
#include "ceammc_factory.h"

constexpr int OUTLET_DATA = 0;
constexpr int OUTLET_OTHER = 1;

RouteData::RouteData(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();

    type_ = new SymbolProperty("@type", &s_);
    type_->setArgIndex(0);
    type_->setInitOnly();
    addProperty(type_);
}

void RouteData::initDone()
{
    onLoadBang();
}

void RouteData::onLoadBang()
{
    if (type_->value() != &s_ && data_id_ == data::DATA_INVALID) {
        auto id = DataStorage::instance().typeByName(type_->value()->s_name);
        if (id == data::DATA_INVALID)
            OBJ_ERR << "datatype not found: " << type_->value()->s_name;
        else
            data_id_ = id;
    }
}

void RouteData::onBang()
{
    bangTo(OUTLET_OTHER);
}

void RouteData::onFloat(t_float f)
{
    floatTo(OUTLET_OTHER, f);
}

void RouteData::onSymbol(t_symbol* s)
{
    symbolTo(OUTLET_OTHER, s);
}

void RouteData::onList(const AtomListView& lv)
{
    listTo(OUTLET_OTHER, lv);
}

void RouteData::onData(const Atom& data)
{
    if (data_id_ == data::DATA_INVALID)
        atomTo(OUTLET_DATA, data);
    else if (data.isDataType(data_id_))
        atomTo(OUTLET_DATA, data);
    else
        atomTo(OUTLET_OTHER, data);
}

void RouteData::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(OUTLET_OTHER, s, lv);
}

void setup_route_data()
{
    ObjectFactory<RouteData> obj("route.data");
    obj.addAlias("route.d");
    obj.processData();
    obj.useLoadBang();

    obj.setXletsInfo({ "any" }, { "data", "other messages" });
    obj.noPropsDispatch();

    obj.setDescription("separate data messages from other types");
    obj.setCategory("flow");
    obj.setKeywords({ "route", "data" });
}
